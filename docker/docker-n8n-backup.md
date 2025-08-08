
In n8n’s case, the **critical part is the persistent data volume** (`n8n_data`) which stores all workflows, credentials, executions, and settings.

---

## **1. What Needs to Be Backed Up**

From your `docker-compose.yml`:

| Component                                                 | What to Back Up                           | Why                                                         |
| --------------------------------------------------------- | ----------------------------------------- | ----------------------------------------------------------- |
| **`n8n_data` volume**                                     | `/home/node/.n8n` inside container        | All workflows, credentials (encrypted), execution logs      |
| **`traefik_letsencrypt` volume**                          | `/letsencrypt/acme.json` inside container | Stores TLS/SSL certificates from Let's Encrypt              |
| **`docker-compose.yml` + project files**                  | Entire `n8n-docker` folder                | Keeps service definitions, labels, networks, Python scripts |
| **Custom scripts/data** (`python-scripts`, `shared-data`) | Folder contents                           | Your own automation logic & execution logs                  |

---

## **2. Backup Methods**

### **Option A – Using `docker run --rm` (Recommended)**

This method copies Docker volumes to `.tar.gz` files without stopping containers.

#### **Backup Script**

```bash
#!/bin/bash
set -e

BACKUP_DIR=~/n8n_backups/$(date +%F_%H-%M-%S)
mkdir -p "$BACKUP_DIR"

echo "📦 Backing up n8n_data volume..."
docker run --rm \
  -v n8n-docker_n8n_data:/data \
  -v "$BACKUP_DIR":/backup \
  alpine tar czf /backup/n8n_data.tar.gz -C /data .

echo "📦 Backing up traefik_letsencrypt volume..."
docker run --rm \
  -v n8n-docker_traefik_letsencrypt:/data \
  -v "$BACKUP_DIR":/backup \
  alpine tar czf /backup/traefik_letsencrypt.tar.gz -C /data .

echo "📦 Backing up project files..."
tar czf "$BACKUP_DIR/project_files.tar.gz" -C ~/n8n-docker .

echo "✅ Backup completed. Saved in: $BACKUP_DIR"
```

* Save this as `backup.sh`
* Run:

  ```bash
  chmod +x backup.sh
  ./backup.sh
  ```

---

### **Option B – Hot Backup with `docker cp`**

You can also copy the live data directly from containers:

```bash
docker cp n8n:/home/node/.n8n ./n8n_data_backup
docker cp traefik:/letsencrypt ./traefik_letsencrypt_backup
```

But **tar + volume mount method is safer** for automation.

---

## **3. Restore Procedure**

To restore on the same or another machine:

```bash
# Stop containers
docker compose down

# Restore n8n data volume
docker run --rm \
  -v n8n-docker_n8n_data:/data \
  -v $(pwd):/backup \
  alpine sh -c "rm -rf /data/* && tar xzf /backup/n8n_data.tar.gz -C /data"

# Restore traefik certs volume
docker run --rm \
  -v n8n-docker_traefik_letsencrypt:/data \
  -v $(pwd):/backup \
  alpine sh -c "rm -rf /data/* && tar xzf /backup/traefik_letsencrypt.tar.gz -C /data"

# Restore project files
tar xzf project_files.tar.gz -C ~/n8n-docker

# Start containers
docker compose up -d
```

---

## **4. Backup Schedule**

* Use **cron job** to run backup daily at midnight:

```bash
crontab -e
```

Add:

```cron
0 0 * * * /home/youruser/n8n-docker/backup.sh >> /home/youruser/n8n-docker/backup.log 2>&1
```

---

## **5. Important Notes**

* **Encryption key**:
  n8n encrypts credentials using a key stored in `/home/node/.n8n/config` (inside `n8n_data`). If you lose this, you can’t decrypt credentials.
* **Test restore**:
  Always try restoring to a test server before relying on it.
* **Remote backups**:
  Sync `~/n8n_backups` to Google Drive, S3, or similar for offsite safety.
* **Downtime**:
  Volumes can be backed up live, but for **absolutely consistent state**, run:

  ```bash
  docker compose down
  ./backup.sh
  docker compose up -d
  ```

---

Here’s the structured approach:

---

## **1. Core Principles for Reliable Backups**

Think of backup strategy like protecting a treasure chest:

| Principle             | Why it matters for n8n                                           | Practical Action                                               |
| --------------------- | ---------------------------------------------------------------- | -------------------------------------------------------------- |
| **Multiple Copies**   | Hardware failure or accidental deletion can destroy one copy     | Keep at least 3 copies (local, remote, offsite)                |
| **Versioned Backups** | A bad config or corrupt DB should not overwrite your only backup | Use time-stamped folders and retain history                    |
| **Automation**        | Manual backups fail when you forget                              | Cron jobs, CI pipelines, or backup software                    |
| **Offsite Storage**   | Server crash, theft, or data center fire                         | Use S3, Google Drive, Backblaze B2, or rsync to another server |
| **Tested Restores**   | A backup you can’t restore is useless                            | Regularly restore to a staging server                          |
| **Data Encryption**   | n8n stores sensitive credentials                                 | Encrypt backups before storing remotely                        |

---

## **2. What to Always Back Up in n8n**

Your `docker-compose.yml` shows exactly what’s critical:

| Component                                                                      | Backup Method         | Why                                                 |
| ------------------------------------------------------------------------------ | --------------------- | --------------------------------------------------- |
| **`n8n_data` volume** (`/home/node/.n8n`)                                      | Docker volume tarball | Contains workflows, encrypted credentials, settings |
| **`traefik_letsencrypt` volume**                                               | Docker volume tarball | Stores SSL/TLS certificates                         |
| **Project Files** (`docker-compose.yml`, Python scripts, shared-data, configs) | File system tarball   | Recreates deployment exactly as before              |
| **Environment Variables** (`.env` if exists)                                   | File copy             | Needed for same configuration after restore         |

---

## **3. Recommended Backup Workflow**

### **Step 1 – Snapshot the Volumes**

Volumes are the heart of n8n. Back up them daily:

```bash
docker run --rm -v n8n-docker_n8n_data:/data -v $(pwd):/backup alpine tar czf /backup/n8n_data_$(date +%F).tar.gz -C /data .
docker run --rm -v n8n-docker_traefik_letsencrypt:/data -v $(pwd):/backup alpine tar czf /backup/traefik_letsencrypt_$(date +%F).tar.gz -C /data .
```

---

### **Step 2 – Backup Project Files**

```bash
tar czf project_files_$(date +%F).tar.gz ~/n8n-docker
```

---

### **Step 3 – Encrypt the Backups**

Use **GPG** before storing remotely:

```bash
gpg --symmetric --cipher-algo AES256 n8n_data_2025-08-09.tar.gz
```

This prevents anyone from reading credentials even if they get the backup.

---

### **Step 4 – Store in Multiple Locations**

* **Local**: Keep the latest 7 days locally for fast restore.
* **Cloud**: Push to S3/Backblaze B2/Google Drive.
* **Optional Cold Storage**: Monthly backup to external HDD kept offline.

Example: Upload to AWS S3

```bash
aws s3 cp n8n_data_2025-08-09.tar.gz s3://my-n8n-backups/
```

---

### **Step 5 – Automate with Cron**

Daily at 2 AM:

```cron
0 2 * * * /home/user/n8n-docker/backup.sh >> /var/log/n8n_backup.log 2>&1
```

---

## **4. Advanced & Recommended Enhancements**

| Enhancement                                                       | Benefit                                       |
| ----------------------------------------------------------------- | --------------------------------------------- |
| **Pre-stop container for backups**                                | Prevents file write collisions                |
| **Docker Volume Backup Tools** (`loomchild/volume-backup`)        | Easier and faster volume backups              |
| **Staging Restore Tests**                                         | Verify backups monthly on a different machine |
| **Incremental Backups with Restic or BorgBackup**                 | Saves space, keeps history                    |
| **Monitoring & Alerts**                                           | Email/Slack if backup fails                   |
| **Keep Backup of Encryption Key** (`encryptionKey` in n8n config) | Without it, you can’t decrypt credentials     |

---

## **5. Example “Gold Standard” Backup Setup**

Here’s what a **production-grade n8n backup setup** looks like:

```
Nightly (2 AM):
 ├── Stop n8n container (optional)
 ├── Dump n8n_data volume → tar.gz → encrypt → upload to:
 │       ├── AWS S3 (Versioned bucket, lifecycle policy to Glacier after 30 days)
 │       ├── Local NAS
 │
 ├── Dump traefik_letsencrypt volume → tar.gz → encrypt → same storage
 ├── Backup project files & .env
 ├── Restart containers
 └── Send success/failure notification to Slack
```

Retention policy:

* Keep **7 daily**, **4 weekly**, **12 monthly** backups in S3
* Glacier deep archive for yearly snapshots

---

Below are **ready-to-run, step-by-step commands (manual)** and **two complete Python scripts** (`backup.py`, `restore.py`) that you can drop into your `~/n8n-docker` server.
Scripts cover hot backups (no downtime) and optional cold backups (stop containers first), optional GPG encryption, and optional S3 upload. I also include cron/systemd examples and verification steps.



# Overview (quick)

* **Manual commands:** copy Docker volumes to `.tar.gz`, encrypt with `gpg`, upload to S3 or copy offsite.
* **Python scripts:** automate the exact commands (use `docker run --rm -v <volume>:/data -v <hostdir>:/backup alpine tar ...` under the hood).
* **Restore script:** stops containers (optional), decrypts (if needed) and restores tarballs into Docker volumes, then starts containers.

---

# Prerequisites (install once)

Run on your Ubuntu server where docker-compose runs:

```bash
# Docker, compose already present (you have them). Install utilities:
sudo apt update
sudo apt install -y gnupg python3 python3-pip tar gzip

# Python libs (only needed if using S3 upload in script)
python3 -m pip install --upgrade pip boto3
```

Ensure AWS creds are set if using S3:

```bash
export AWS_ACCESS_KEY_ID=...
export AWS_SECRET_ACCESS_KEY=...
export AWS_DEFAULT_REGION=us-east-1
```

Check actual Docker volume names (do this before running scripts):

```bash
docker volume ls
# or to inspect:
docker volume inspect n8n_data
```

> If your compose project name prefixes volumes (e.g. `n8n-docker_n8n_data`), use that exact volume name in the scripts or pass it as argument.

---

# Manual step-by-step commands (Hot backup — no downtime)

**Set vars**

```bash
# run from your project folder, e.g. ~/n8n-docker
BACKUP_DIR=~/n8n_backups/$(date +%F_%H-%M-%S)
mkdir -p "$BACKUP_DIR"
TIMESTAMP=$(date +%F_%H-%M-%S)
```

**Volume backup (n8n\_data and traefik\_letsencrypt)**
Replace `n8n_data` with the exact name from `docker volume ls` if prefixed.

```bash
# backup n8n_data volume
docker run --rm \
  -v n8n_data:/data \
  -v "$BACKUP_DIR":/backup \
  alpine sh -c "tar czf /backup/n8n_data_${TIMESTAMP}.tar.gz -C /data ."

# backup traefik_letsencrypt volume
docker run --rm \
  -v traefik_letsencrypt:/data \
  -v "$BACKUP_DIR":/backup \
  alpine sh -c "tar czf /backup/traefik_letsencrypt_${TIMESTAMP}.tar.gz -C /data ."
```

**Backup project files (compose, scripts, shared-data)**

```bash
tar czf "$BACKUP_DIR/project_files_${TIMESTAMP}.tar.gz" -C ~/n8n-docker .
```

**(Optional) Symmetric encrypt each tarball with GPG**

```bash
# set passphrase in env or prompt
export GPG_PASSPHRASE="your-strong-passphrase"
for f in "$BACKUP_DIR"/*.tar.gz; do
  gpg --batch --yes --passphrase-fd 0 --symmetric --cipher-algo AES256 --output "${f}.gpg" "$f" <<<"$GPG_PASSPHRASE"
  shred -u "$f"   # securely remove plaintext archive (optional)
done
```

**(Optional) Upload to S3**

```bash
aws s3 cp "$BACKUP_DIR" s3://my-n8n-backups/ --recursive --exclude "*" --include "*.gpg"
# Or upload plain tar.gz if you didn't encrypt
```

**Verify backup contents (sample)**

```bash
# list files in a tarball (without extracting)
tar tzf "$BACKUP_DIR/n8n_data_${TIMESTAMP}.tar.gz" | head -n 40
```

---

# Manual restore (Cold / safest approach)

**1. Stop docker-compose (recommended for consistency)**

```bash
# run from project folder containing docker-compose.yml
docker compose down
# OR stop only n8n:
# docker compose stop n8n
```

**2. Copy backup files to server and (if encrypted) decrypt**

```bash
# decrypt
gpg --batch --yes --passphrase-fd 0 --output /tmp/n8n_data_restore.tar.gz --decrypt /path/to/n8n_data_2025-08-08_02-00-00.tar.gz.gpg <<<"$GPG_PASSPHRASE"
```

**3. Restore tarball into volume**

```bash
docker run --rm \
  -v n8n_data:/data \
  -v /tmp:/backup \
  alpine sh -c "rm -rf /data/* && tar xzf /backup/n8n_data_restore.tar.gz -C /data"

docker run --rm \
  -v traefik_letsencrypt:/data \
  -v /tmp:/backup \
  alpine sh -c "rm -rf /data/* && tar xzf /backup/traefik_letsencrypt_restore.tar.gz -C /data"
```

**4. Restore project files (if needed)**

```bash
tar xzf /path/to/project_files_2025-08-08.tar.gz -C ~/n8n-docker
```

**5. Start compose**

```bash
docker compose up -d
```

**6. Verify n8n is healthy**

```bash
# check logs
docker compose logs -f n8n
# check UI & workflows, validate credentials decrypt properly in n8n
```

---

# Python automation scripts

Below are **complete** scripts. Put them in `~/n8n-docker/backup.py` and `~/n8n-docker/restore.py`. They assume the server has `docker`, `gpg`, and optionally `aws` credentials configured.

---

## `backup.py` — full script

Save as `backup.py` and `chmod +x backup.py`.

```python
#!/usr/bin/env python3
"""
backup.py
Automates Docker volume backups for n8n + project files.
Features:
 - Creates timestamped backup directory
 - Archives named Docker volumes using an ephemeral alpine container
 - Optionally encrypts archives with gpg (symmetric)
 - Optionally uploads to S3 (boto3 required)
 - Optionally prunes local backups older than retention days

Usage examples:
  ./backup.py --volumes n8n_data traefik_letsencrypt --project-dir ~/n8n-docker --backup-root ~/n8n_backups --encrypt --retain 14 --s3-bucket my-n8n-backups
"""

import argparse
import datetime
import os
import subprocess
import sys
import shutil
from pathlib import Path
import glob
import logging

# Optional boto3 import (only needed if using --s3-bucket)
try:
    import boto3  # type: ignore
    from botocore.exceptions import BotoCoreError, NoCredentialsError  # type: ignore
except Exception:
    boto3 = None

logging.basicConfig(level=logging.INFO, format="%(asctime)s %(levelname)s: %(message)s")

def run(cmd, capture_output=False):
    logging.debug("RUN: %s", " ".join(cmd))
    subprocess.run(cmd, check=True, stdout=(subprocess.PIPE if capture_output else None))

def timestamp():
    return datetime.datetime.utcnow().strftime("%Y-%m-%d_%H-%M-%S")

def backup_volume(volume_name: str, dest_dir: Path) -> Path:
    """Backup a docker volume to dest_dir. Returns the tar.gz Path."""
    fname = f"{volume_name}_{timestamp()}.tar.gz"
    dest = dest_dir / fname
    dest_dir.mkdir(parents=True, exist_ok=True)
    cmd = [
        "docker", "run", "--rm",
        "-v", f"{volume_name}:/data",
        "-v", f"{str(dest_dir)}:/backup",
        "alpine", "sh", "-c",
        f"tar czf /backup/{fname} -C /data ."
    ]
    logging.info("Backing up volume %s -> %s", volume_name, dest)
    run(cmd)
    return dest

def backup_project(project_dir: str, dest_dir: Path) -> Path:
    """Archive the project folder."""
    p = Path(project_dir).expanduser().resolve()
    if not p.exists():
        raise FileNotFoundError(f"Project dir not found: {p}")
    fname = f"project_files_{timestamp()}.tar.gz"
    dest = dest_dir / fname
    logging.info("Archiving project dir %s -> %s", p, dest)
    run(["tar", "czf", str(dest), "-C", str(p), "."])
    return dest

def gpg_encrypt(file_path: Path, passphrase: str) -> Path:
    """Encrypt file with gpg symmetric. Returns path to .gpg file."""
    out = file_path.with_suffix(file_path.suffix + ".gpg")
    logging.info("Encrypting %s -> %s", file_path, out)
    # use passphrase via stdin (fd 0)
    cmd = ["gpg", "--batch", "--yes", "--passphrase-fd", "0", "--symmetric", "--cipher-algo", "AES256", "--output", str(out), str(file_path)]
    subprocess.run(cmd, check=True, input=passphrase.encode())
    return out

def upload_s3(file_path: Path, bucket: str, s3_prefix: str = ""):
    if boto3 is None:
        raise RuntimeError("boto3 not installed; cannot upload to S3")
    s3 = boto3.client("s3")
    key = (s3_prefix.rstrip("/") + "/" if s3_prefix else "") + file_path.name
    logging.info("Uploading %s -> s3://%s/%s", file_path, bucket, key)
    try:
        s3.upload_file(str(file_path), bucket, key)
    except (BotoCoreError, NoCredentialsError) as e:
        logging.error("S3 upload failed: %s", e)
        raise

def prune_local(backups_root: Path, retain_days: int):
    """Remove backup files older than retain_days"""
    cutoff = datetime.datetime.utcnow().timestamp() - (retain_days * 86400)
    logging.info("Pruning files older than %d days in %s", retain_days, backups_root)
    for f in backups_root.glob("**/*"):
        if f.is_file():
            if f.stat().st_mtime < cutoff:
                logging.info("Removing old file: %s", f)
                f.unlink()

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--volumes", nargs="+", required=True, help="Docker volume names to backup")
    parser.add_argument("--project-dir", default=".", help="Project directory to archive (docker-compose.yml + scripts)")
    parser.add_argument("--backup-root", default="~/n8n_backups", help="Root dir to place backups")
    parser.add_argument("--encrypt", action="store_true", help="Encrypt resulting archives with gpg (symmetric). Passphrase via env GPG_PASSPHRASE")
    parser.add_argument("--s3-bucket", help="If provided, upload encrypted files to this S3 bucket")
    parser.add_argument("--s3-prefix", default="", help="S3 key prefix")
    parser.add_argument("--retain", type=int, default=30, help="Retain local backups (days)")
    parser.add_argument("--stop-containers", action="store_true", help="Stop docker compose before backup (safer)")
    parser.add_argument("--compose-file", default="./docker-compose.yml", help="docker-compose file path (used with --stop-containers)")
    args = parser.parse_args()

    backups_root = Path(args.backup_root).expanduser().resolve()
    backups_root.mkdir(parents=True, exist_ok=True)

    try:
        if args.stop_containers:
            logging.info("Stopping docker-compose using file: %s", args.compose_file)
            run(["docker", "compose", "-f", args.compose_file, "down"])

        created = []
        for vol in args.volumes:
            t = backup_volume(vol, backups_root)
            created.append(t)

        proj = backup_project(args.project_dir, backups_root)
        created.append(proj)

        encrypted_files = []
        if args.encrypt:
            passphrase = os.environ.get("GPG_PASSPHRASE")
            if not passphrase:
                logging.error("GPG_PASSPHRASE env var not set")
                sys.exit(2)
            for f in created:
                enc = gpg_encrypt(f, passphrase)
                encrypted_files.append(enc)
                # optionally remove plaintext
                try:
                    f.unlink()
                except Exception:
                    pass

        if args.s3_bucket:
            # prefer to upload encrypted files if encryption enabled
            to_upload = encrypted_files if encrypted_files else created
            for f in to_upload:
                upload_s3(f, args.s3_bucket, args.s3_prefix)

        prune_local(backups_root, args.retain)

        if args.stop_containers:
            logging.info("Starting docker-compose back up")
            run(["docker", "compose", "-f", args.compose_file, "up", "-d"])

        logging.info("All done. Backups stored in %s", backups_root)

    except subprocess.CalledProcessError as e:
        logging.error("Command failed: %s", e)
        sys.exit(1)

if __name__ == "__main__":
    main()
```

**Usage examples**

```bash
# hot backup, encrypt, upload to S3, retain 14 days
export GPG_PASSPHRASE="veryStrongPassphrase"
./backup.py --volumes n8n_data traefik_letsencrypt --project-dir ~/n8n-docker --backup-root ~/n8n_backups --encrypt --s3-bucket my-n8n-backups --retain 14

# Cold backup (stop containers first)
sudo ./backup.py --volumes n8n_data traefik_letsencrypt --stop-containers --compose-file ./docker-compose.yml
```

---

## `restore.py` — full script

Save as `restore.py` and `chmod +x restore.py`.

```python
#!/usr/bin/env python3
"""
restore.py
Restore Docker volumes from backup tarballs (optionally encrypted with gpg).
Usage:
  ./restore.py --backup-dir ~/n8n_backups --volume-map n8n_data:n8n_data_2025-08-08_02-00-00.tar.gz \
               --compose-file ./docker-compose.yml --stop-containers --gpg
Notes:
 - backup files can be plain tar.gz or .tar.gz.gpg (if encrypted)
 - script will remove contents of target volume before extracting
"""

import argparse
import subprocess
import os
from pathlib import Path
import logging
import tempfile

logging.basicConfig(level=logging.INFO, format="%(asctime)s %(levelname)s: %(message)s")

def run(cmd):
    logging.debug("RUN: %s", " ".join(cmd))
    subprocess.run(cmd, check=True)

def decrypt_gpg(enc_path: Path, passphrase: str) -> Path:
    tmp = Path(tempfile.mkdtemp()) / enc_path.with_suffix('').name
    logging.info("Decrypting %s -> %s", enc_path, tmp)
    run(["gpg", "--batch", "--yes", "--passphrase-fd", "0", "--output", str(tmp), "--decrypt", str(enc_path)],)
    return tmp

def restore_to_volume(tarball: Path, volume_name: str):
    """Restore tarball into a docker volume (clears existing content)."""
    logging.info("Restoring %s -> volume %s", tarball, volume_name)
    # ensure parent dir is absolute (we will mount it)
    host_dir = str(tarball.parent.resolve())
    fname = tarball.name
    cmd = [
        "docker", "run", "--rm",
        "-v", f"{volume_name}:/data",
        "-v", f"{host_dir}:/backup",
        "alpine", "sh", "-c",
        f"rm -rf /data/* && tar xzf /backup/{fname} -C /data"
    ]
    run(cmd)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--backup-dir", required=True, help="Directory containing backups")
    parser.add_argument("--volume-map", nargs="+", required=True,
                        help="List like: n8n_data:n8n_data_2025-08-09_02-00-00.tar.gz traefik_letsencrypt:traefik_letsencrypt_...")
    parser.add_argument("--compose-file", default="./docker-compose.yml")
    parser.add_argument("--stop-containers", action="store_true", help="Stop docker compose before restore")
    parser.add_argument("--gpg", action="store_true", help="Indicates backups are encrypted with gpg; will prompt for passphrase via GPG_PASSPHRASE env var")
    args = parser.parse_args()

    backup_dir = Path(args.backup_dir).expanduser().resolve()
    if not backup_dir.exists():
        raise SystemExit("Backup dir not found: " + str(backup_dir))

    if args.stop_containers:
        logging.info("Stopping docker compose (file=%s)", args.compose_file)
        run(["docker", "compose", "-f", args.compose_file, "down"])

    for mapping in args.volume_map:
        if ":" not in mapping:
            raise SystemExit("Invalid volume-map entry: " + mapping)
        volume, fname = mapping.split(":", 1)
        fpath = backup_dir / fname
        if args.gpg:
            passphrase = os.environ.get("GPG_PASSPHRASE")
            if not passphrase:
                raise SystemExit("Set GPG_PASSPHRASE env var before running")
            # if file ends with .gpg do decrypt, otherwise attempt to decrypt whatever
            decrypted = decrypt_gpg(fpath, passphrase)
            try:
                restore_to_volume(decrypted, volume)
            finally:
                try:
                    decrypted.unlink()
                except Exception:
                    pass
        else:
            restore_to_volume(fpath, volume)

    logging.info("Restore finished. Start containers now.")
    run(["docker", "compose", "-f", args.compose_file, "up", "-d"])
    logging.info("Containers started.")

if __name__ == "__main__":
    main()
```

**Example usage**

```bash
# restore encrypted backup for n8n_data and traefik volume
export GPG_PASSPHRASE="veryStrongPassphrase"
./restore.py --backup-dir ~/n8n_backups --volume-map \
  "n8n_data:n8n_data_2025-08-09_02-00-00.tar.gz.gpg" \
  "traefik_letsencrypt:traefik_letsencrypt_2025-08-09_02-00-00.tar.gz.gpg" \
  --compose-file ./docker-compose.yml --stop-containers --gpg
```

---

# Cron / systemd scheduling examples

**Crontab (daily @ 02:00)**

```bash
# edit crontab
crontab -e

# add line (adjust user & paths)
0 2 * * * /home/n8nuser/n8n-docker/backup.py --volumes n8n_data traefik_letsencrypt --project-dir /home/n8nuser/n8n-docker --backup-root /home/n8nuser/n8n_backups --encrypt --s3-bucket my-n8n-backups >> /home/n8nuser/n8n-docker/backup.log 2>&1
```

**systemd service + timer** (optional, more robust than cron).

`/etc/systemd/system/n8n-backup.service`

```ini
[Unit]
Description=n8n Backup Service

[Service]
Type=oneshot
User=n8nuser
WorkingDirectory=/home/n8nuser/n8n-docker
ExecStart=/home/n8nuser/n8n-docker/backup.py --volumes n8n_data traefik_letsencrypt --project-dir /home/n8nuser/n8n-docker --backup-root /home/n8nuser/n8n_backups --encrypt --s3-bucket my-n8n-backups
```

`/etc/systemd/system/n8n-backup.timer`

```ini
[Unit]
Description=Run n8n backup daily

[Timer]
OnCalendar=daily
Persistent=true

[Install]
WantedBy=timers.target
```

```bash
sudo systemctl daemon-reload
sudo systemctl enable --now n8n-backup.timer
```

---

# Important notes, gotchas & best-practice checklist

* **Credentials/encryption key**: If you let n8n encrypt credentials with an `encryptionKey` in its settings, that key is stored under `/home/node/.n8n` — **back up that folder** (covered by `n8n_data` volume). Losing that key means credentials are unrecoverable.
* **Test restores monthly**: Always restore to a staging host to validate.
* **If you use Postgres/MySQL externally**: Use `pg_dump`/`mysqldump` instead of just copying the volume.
* **Consistent backups:** Stopping containers (`docker compose down`) gives crash-consistent backups. Hot backups are fine but may miss in-flight writes.
* **Retention & lifecycle:** Keep 7 daily, 4 weekly, 12 monthly snapshots; use S3 lifecycle rules for archiving.
* **Alerting:** Integrate health checks and e-mail/Slack alerts on failure (systemd + mail, or a small wrapper that posts to webhook).
* **Permissions:** If you see permission issues after restore, run an in-container `chown` as needed (uid for `node` usually 1000):

  ```bash
  docker run --rm -v n8n_data:/data alpine chown -R 1000:1000 /data
  ```

---

# Quick checklist before you run anything

1. `docker volume ls` → confirm volume names.
2. Export `GPG_PASSPHRASE` if using encryption.
3. Ensure AWS creds in env (\~/.aws/ or env) if using S3.
4. Run a **dry run** on a test volume or less critical data first.

---



