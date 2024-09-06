# Git Reset vs Git Revert: Comprehensive Guide

Understanding how to effectively manage your Git history is crucial for maintaining a clean and collaborative workflow. This guide combines **`git reset`** and **`git revert`** commands, illustrating their usage through various scenarios with before and after Git history visualizations.

---

## Table of Contents

1. [Git Reset Overview](#git-reset-overview)
   - [`git reset` Options](#git-reset-options)
   - [Scenario: Soft Reset](#scenario-soft-reset)
   - [Scenario: Mixed Reset](#scenario-mixed-reset)
   - [Scenario: Hard Reset](#scenario-hard-reset)
2. [Git Revert Overview](#git-revert-overview)
   - [Scenario: Single Commit Revert](#scenario-single-commit-revert)
   - [Scenario: Multiple Commits Revert](#scenario-multiple-commits-revert)
   - [Scenario: Reverting a Merge Commit](#scenario-reverting-a-merge-commit)
3. [Combined Scenarios](#combined-scenarios)
4. [Best Practices](#best-practices)

---

## Git Reset Overview

`git reset` is a powerful command that moves the **HEAD** and branch pointer to a specified commit. It can modify the commit history, which makes it suitable for local changes but potentially disruptive for shared branches.

### `git reset` Options

```bash
git reset [--soft | --mixed | --hard] [commit]
```

- **`--soft`**: Moves HEAD to the specified commit, keeping changes staged.
- **`--mixed`** *(default)*: Moves HEAD, resets the staging area, but keeps working directory changes.
- **`--hard`**: Moves HEAD, resets staging area and working directory to match the specified commit.

---

### Scenario: Soft Reset

**Objective**: Undo the last commit but keep changes staged for modification.

#### Before Reset

```bash
$ git log --oneline
abc123 (HEAD -> main) Add payment feature
def456 Add user validation
ghi789 Initial commit
```

#### Commands

```bash
# Undo the last commit, keep changes staged
git reset --soft HEAD~1
```

#### After Reset

```bash
$ git log --oneline
def456 (HEAD -> main) Add user validation
ghi789 Initial commit

$ git status
Changes to be committed:
  modified:   payment.js
  modified:   user.js
```

**Explanation**:

- The `--soft` reset moves the `HEAD` back by one commit (`HEAD~1`), effectively removing `abc123` from the commit history.
- Changes from the undone commit remain **staged**, allowing you to modify or recommit them.

---

### Scenario: Mixed Reset

**Objective**: Unstage the last commit but keep changes in the working directory.

#### Before Reset

```bash
$ git log --oneline
abc123 (HEAD -> main) Add multiple files
def456 Add user validation
ghi789 Initial commit
```

```bash
$ git status
Changes to be committed:
  new file:   file1.js
  new file:   file2.js
```

#### Commands

```bash
# Undo the last commit, unstage changes
git reset HEAD~1
```

#### After Reset

```bash
$ git log --oneline
def456 (HEAD -> main) Add user validation
ghi789 Initial commit

$ git status
Untracked files:
  file1.js
  file2.js
```

**Explanation**:

- The default `--mixed` reset unstages changes from the last commit.
- The commit `abc123` is removed from history, but the changes remain in the working directory as **untracked files**.

---

### Scenario: Hard Reset

**Objective**: Completely remove the last two commits and discard all associated changes.

#### Before Reset

```bash
$ git log --oneline --graph
* abc123 (HEAD -> main) Broken feature
* def456 Working state
* ghi789 Initial commit
```

#### Commands

```bash
# Undo the last two commits and discard changes
git reset --hard HEAD~2
```

#### After Reset

```bash
$ git log --oneline --graph
* def456 (HEAD -> main) Working state
* ghi789 Initial commit

$ git status
On branch main
nothing to commit, working tree clean
```

**Explanation**:

- The `--hard` reset removes the last two commits (`abc123` and `def456`) entirely.
- Both the **staging area** and **working directory** are reset to match the `HEAD`, eliminating all changes from those commits.

---

## Git Revert Overview

Unlike `git reset`, `git revert` creates new commits that **undo** the changes introduced by previous commits. This approach preserves the commit history, making it safe for shared or public branches.

---

### Scenario: Single Commit Revert

**Objective**: Revert the last commit without altering the commit history.

#### Before Revert

```bash
$ git log --oneline
abc123 (HEAD -> main) Add feature X
def456 Add user validation
ghi789 Initial commit
```

#### Commands

```bash
# Revert the last commit
git revert HEAD
```

#### After Revert

```bash
$ git log --oneline
jkl012 (HEAD -> main) Revert "Add feature X"
abc123 Add feature X
def456 Add user validation
ghi789 Initial commit
```

**Explanation**:

- `git revert HEAD` creates a new commit `jkl012` that undoes the changes from `abc123`.
- The original commit `abc123` remains in the history, ensuring a transparent and traceable timeline.

---

### Scenario: Multiple Commits Revert

**Objective**: Revert the last three commits sequentially.

#### Before Revert

```bash
$ git log --oneline
abc123 (HEAD -> main) Feature C
def456 Feature B
ghi789 Feature A
jkl012 Initial commit
```

#### Commands

```bash
# Revert the last three commits
git revert HEAD~2..HEAD
```

#### After Revert

```bash
$ git log --oneline
mno345 (HEAD -> main) Revert "Feature C"
pqr678 Revert "Feature B"
abc123 Feature C
def456 Feature B
ghi789 Feature A
jkl012 Initial commit
```

**Explanation**:

- The range `HEAD~2..HEAD` specifies the last three commits (`abc123`, `def456`, `ghi789`).
- Each of these commits is individually reverted, resulting in new revert commits (`mno345` and `pqr678`).

---

### Scenario: Reverting a Merge Commit

**Objective**: Revert a merge commit while specifying the parent.

#### Before Revert

```bash
$ git log --graph --oneline
*   abc123 (HEAD -> main) Merge feature-x into main
|\
| * def456 Add feature-x
|/
* ghi789 Base commit
```

#### Commands

```bash
# Revert the merge commit abc123 specifying parent 1
git revert -m 1 abc123
```

#### After Revert

```bash
$ git log --graph --oneline
* jkl012 (HEAD -> main) Revert "Merge feature-x into main"
*   abc123 Merge feature-x into main
|\
| * def456 Add feature-x
|/
* ghi789 Base commit
```

**Explanation**:

- The `-m 1` flag indicates that the first parent of the merge commit (`abc123`) should be considered the mainline.
- A new revert commit `jkl012` is created, effectively undoing the merge without altering the existing commit history.

---

## Combined Scenarios

### Scenario 1: Fixing Commits on the Wrong Branch

**Objective**: Move a commit from `main` to `feature-branch`.

#### Before Fix

```bash
# On main branch
$ git log --oneline
abc123 (HEAD -> main) Feature meant for dev
def456 Base commit
```

#### Commands

```bash
# Create and switch to feature branch
git branch feature-branch
git checkout feature-branch

# Reset main to remove the accidental commit
git checkout main
git reset --hard HEAD~1
```

#### After Fix

```bash
# On main branch
$ git log --oneline
def456 (HEAD -> main) Base commit

# On feature-branch
$ git log --oneline
abc123 (feature-branch) Feature meant for dev
def456 Base commit
```

**Explanation**:

- **Reset**: `git reset --hard HEAD~1` removes the accidental commit from `main`.
- **Branching**: `git branch feature-branch` and `git checkout feature-branch` preserves the commit on `feature-branch`.

---

### Scenario 2: Cleaning the Working Directory

**Objective**: Discard all uncommitted changes and reset to the latest remote state.

#### Before Reset

```bash
$ git log --oneline
abc123 (HEAD -> main) Update README
def456 Add new feature
ghi789 Initial commit

$ git status
On branch main
Changes not staged for commit:
  modified:   README.md
  modified:   feature.js
```

#### Commands

```bash
# Discard all local changes
git reset --hard HEAD

# Or reset to match remote
git fetch origin
git reset --hard origin/main
```

#### After Reset

```bash
$ git log --oneline
abc123 (HEAD -> main) Update README
def456 Add new feature
ghi789 Initial commit

$ git status
On branch main
nothing to commit, working tree clean
```

**Explanation**:

- **Hard Reset**: `git reset --hard HEAD` removes all uncommitted changes.
- **Sync with Remote**: Fetching and hard resetting ensures the local branch matches the remote exactly.

---

### Scenario 3: Undoing Public Commits Safely

**Objective**: Revert commits that have already been pushed to a shared repository.

#### Before Revert

```bash
$ git log --oneline
abc123 (HEAD -> main) Add feature X
def456 Add bug fixes
ghi789 Initial commit
```

#### Commands

```bash
# Revert specific commits
git revert abc123
git revert def456
```

#### After Revert

```bash
$ git log --oneline
jkl012 (HEAD -> main) Revert "Add bug fixes"
mno345 Revert "Add feature X"
abc123 Add feature X
def456 Add bug fixes
ghi789 Initial commit
```

**Explanation**:

- **Reverting**: Creates new commits that undo the changes from `abc123` and `def456` without altering the commit history.
- **Safety**: Suitable for public branches as it avoids rewriting history, preventing conflicts for other collaborators.

---

### Scenario 4: Squashing Multiple Commits

**Objective**: Combine the last three commits into a single commit for a cleaner history.

#### Before Reset

```bash
$ git log --oneline
abc123 (HEAD -> feature) Fix typo
def456 Update tests
ghi789 Add login feature
jkl012 Initial commit
```

#### Commands

```bash
# Squash last three commits into one
git reset --soft HEAD~3
git commit -m "Complete login feature"
```

#### After Reset

```bash
$ git log --oneline
mno345 (HEAD -> feature) Complete login feature
jkl012 Initial commit
```

**Explanation**:

- **Soft Reset**: `git reset --soft HEAD~3` undoes the last three commits while keeping changes staged.
- **Commit**: Creating a new commit consolidates the changes into a single, meaningful commit.

---

### Scenario 5: Removing Sensitive Data

**Objective**: Remove a file containing sensitive information from history.

#### Before Reset/Revert

```bash
$ git log --oneline
abc123 (HEAD -> main) Commit with sensitive data
def456 Add regular feature
ghi789 Initial commit
```

#### Commands

```bash
# If the sensitive file is in the last commit
git reset --hard HEAD~1

# Or, if it's in an older commit, use revert
git revert abc123
```

#### After Reset/Revert

```bash
# After reset
$ git log --oneline
def456 (HEAD -> main) Add regular feature
ghi789 Initial commit

# After revert
$ git log --oneline
jkl012 (HEAD -> main) Revert "Commit with sensitive data"
abc123 Commit with sensitive data
def456 Add regular feature
ghi789 Initial commit
```

**Explanation**:

- **Reset**: Completely removes the last commit containing sensitive data. Suitable if the commit is local and hasn’t been shared.
- **Revert**: Safely undoes the changes introduced by the commit without removing it from history. Ideal for shared or public repositories.

---

## Best Practices

1. **Backup Before Major Operations**

   ```bash
   git branch backup-$(date +%Y%m%d)
   ```

   *Create a backup branch to safeguard your current state before performing resets or reverts.*

2. **Use `--force-with-lease` Instead of `--force`**

   ```bash
   git push --force-with-lease origin main
   ```

   *Safer than `--force`, it ensures you don’t overwrite others’ work.*

3. **Prefer `git revert` for Public Branches**

   *Avoid rewriting history on shared branches to prevent conflicts and maintain collaboration integrity.*

4. **Communicate with Your Team**

   *Before performing operations that alter commit history, inform your team to avoid disruptions.*

5. **Verify with `git log` and `git status`**

   ```bash
   git log --oneline --graph --all
   git status
   ```

   *Always check the repository state after performing resets or reverts.*

6. **Document Significant Changes**

   *Maintain clear commit messages and documentation to provide context for changes, especially after reverts.*

---

## Summary

- **`git reset`**:
  - **Pros**:
    - Powerful for rewriting history
    - Flexible with `--soft`, `--mixed`, and `--hard` options
  - **Cons**:
    - Can be destructive, especially with `--hard`
    - Not recommended for shared branches

- **`git revert`**:
  - **Pros**:
    - Safe for public/shared branches
    - Preserves history by creating new commits
  - **Cons**:
    - Can clutter history with multiple revert commits
    - May require handling merge conflicts during revert

Choosing between `git reset` and `git revert` depends on your workflow needs. Use `git reset` for local branch management and history cleanup, and `git revert` for safely undoing changes in shared repositories.

---

# Additional Resources

- [Git Documentation: git-reset](https://git-scm.com/docs/git-reset)
- [Git Documentation: git-revert](https://git-scm.com/docs/git-revert)
- [Atlassian Git Reset vs Revert](https://www.atlassian.com/git/tutorials/undoing-changes/git-reset)
- [Pro Git Book: Rewriting History](https://git-scm.com/book/en/v2/Git-Tools-Rewriting-History)

---

By mastering `git reset` and `git revert`, you can maintain a clean and efficient Git history, ensuring smooth collaboration and effective project management.