# Managing Collaborator Access in GitLab with Branch Restrictions

To effectively manage collaborators in your private GitLab repository, ensuring they can only modify specific branches (and not critical ones like `main` and `RELEASE`), follow the comprehensive steps below. This guide includes practical scenarios and examples to facilitate understanding and implementation.

## Scenario Overview

Imagine you have a GitLab repository with the following branches:

- **`main`**: The primary stable branch.
- **`RELEASE`**: The branch designated for production-ready code.
- **`feature/*` & `bugfix/*`**: Branches where collaborators can develop new features or fix bugs.

You aim to:

1. **Add Collaborators**: Allow them to clone, create branches, and submit merge requests (MRs).
2. **Restrict Access**: Prevent collaborators from pushing directly to `main` and `RELEASE`.
3. **Control Merge Approvals**: Ensure that only you can approve and merge MRs.

Let's delve into each step with detailed instructions and examples.

---

## Step 1: Adding Collaborators as Project Members

### 1.1. Navigate to Your Project

1. **Log in to GitLab** and select your project from the dashboard or via the **Projects** menu.

### 1.2. Access Members Settings

1. In your project's sidebar, go to **Project information** > **Members**.

### 1.3. Invite Members

1. Click on the **"Invite members"** button.
2. **Enter Collaborator Details**:
   - **Name or Email**: Input the collaborator's GitLab username or their registered email address.
   - **Role**: Select **"Developer"**. This role allows them to:
     - Clone the repository.
     - Create branches.
     - Submit merge requests.
3. **Set Expiry Date (Optional)**: If you want the access to be temporary, set an expiration date.
4. Click **"Invite"** to send the invitation.

### Example

Suppose you want to add a collaborator named **Alice**:

- **Username/Email**: `alice@example.com`
- **Role**: `Developer`
- **Expiry Date**: *(Leave blank for indefinite access)*

---

## Step 2: Protecting Critical Branches (`main` and `RELEASE`)

Protecting branches ensures that only authorized roles can push or merge changes directly.

### 2.1. Access Repository Settings

1. In your project's sidebar, navigate to **Settings** > **Repository**.

### 2.2. Protect the `main` Branch

1. Scroll to the **"Protected branches"** section.
2. In the **"Protect a branch"** dropdown:
   - **Branch**: Enter `main`.
   - **Allowed to merge**: Select **"Maintainers"** (typically only you).
   - **Allowed to push**: Select **"No one"**.
3. Click **"Protect"** to apply the settings.

### 2.3. Protect the `RELEASE` Branch

1. Repeat the steps for the `RELEASE` branch:
   - **Branch**: Enter `RELEASE`.
   - **Allowed to merge**: Select **"Maintainers"**.
   - **Allowed to push**: Select **"No one"**.
2. Click **"Protect"**.

### Visual Representation

```plaintext
Protected Branch: main
- Allowed to merge: Maintainers
- Allowed to push: No one

Protected Branch: RELEASE
- Allowed to merge: Maintainers
- Allowed to push: No one
```

**Result**: Collaborators cannot push directly to `main` or `RELEASE` and cannot merge changes without your approval.

---

## Step 3: Defining Branch Permissions for Collaborators

To allow collaborators to work on specific branches (e.g., `feature/*` and `bugfix/*`), you need to set up rules that permit them to push to these branches.

### 3.1. Using Push Rules

**Push Rules** in GitLab allow you to enforce branch naming conventions and other restrictions.

1. **Access Push Rules**:
   - Navigate to **Settings** > **Repository**.
   - Scroll to the **"Push Rules"** section.

2. **Configure Allowed Branch Patterns**:
   - **Allowed to push**:
     - Enter a regex pattern that matches your desired branch prefixes. For example:
       ```regex
       ^(feature|bugfix)\/.+$
       ```
     - This pattern allows branches that start with `feature/` or `bugfix/`.
   
3. **Save Changes**:
   - Click **"Add push rule"** to apply the settings.

### Example `.gitlab-ci.yml` for Push Rules

While Push Rules are typically set via the GitLab UI, you can also define certain rules in your `.gitlab-ci.yml` for CI/CD processes.

```yaml
stages:
  - validate

validate_push:
  stage: validate
  script:
    - |
      if [[ ! "$CI_COMMIT_BRANCH" =~ ^(feature|bugfix)/.+$ ]]; then
        echo "Invalid branch name. Use feature/* or bugfix/*."
        exit 1
      fi
  only:
    - branches
```

**Explanation**:

- **Stage**: Defines a `validate` stage in the CI/CD pipeline.
- **Script**:
  - Checks if the branch name matches the `feature/*` or `bugfix/*` pattern.
  - If not, the push is rejected with an error message.
- **Only**: Applies this job to all branches.

### 3.2. Alternative: Using Protected Branch Patterns

Instead of Push Rules, you can protect branch patterns to grant specific permissions.

1. **Access Protected Branches**:
   - Navigate to **Settings** > **Repository** > **Protected branches**.

2. **Protect Branch Patterns**:
   - **Branch**: Enter `feature/*`.
   - **Allowed to merge**: Select **"Developers + Maintainers"**.
   - **Allowed to push**: Select **"Developers + Maintainers"**.
   - Click **"Protect"**.
   
   - **Repeat** for `bugfix/*`:
     - **Branch**: `bugfix/*`
     - **Allowed to merge**: `Developers + Maintainers`
     - **Allowed to push**: `Developers + Maintainers`

**Result**: Collaborators with the Developer role can push to `feature/*` and `bugfix/*` branches but cannot modify `main` or `RELEASE`.

---

## Step 4: Configuring Merge Request (MR) Settings

Ensure that all MRs require your approval before being merged into protected branches.

### 4.1. Access Merge Request Approvals

1. Navigate to **Settings** > **General**.
2. Expand the **"Merge request approvals"** section.

### 4.2. Set Approval Requirements

1. **Approval Rules**:
   - **Require approvals**: Set to `1` (minimum number of approvals required).
   - **Approver Groups or Users**:
     - Add yourself as the sole approver.
     - Optionally, add specific groups if needed.

2. **Enforce Approval**:
   - Ensure that MRs cannot be merged without the required approvals.

3. **Save Changes**.

### Example Configuration

```plaintext
Merge Request Approvals:
- Require at least 1 approval.
- Approver: your_username (Maintainer)
```

**Result**: When collaborators submit MRs to protected branches like `main` or `RELEASE`, the MRs will await your approval before merging.

---

## Practical Workflow Example

### 5.1. Collaborator Workflow

1. **Cloning the Repository**:

   ```bash
   git clone git@gitlab.com:yourusername/yourrepo.git
   cd yourrepo
   ```

2. **Creating a New Feature Branch**:

   ```bash
   git checkout -b feature/add-login
   ```

3. **Making Changes and Committing**:

   ```bash
   # Make changes to the codebase
   git add .
   git commit -m "Add login feature"
   ```

4. **Pushing the Feature Branch**:

   ```bash
   git push origin feature/add-login
   ```

   - **Allowed**: Because the branch name matches `feature/*`.

5. **Creating a Merge Request**:

   - Navigate to your project on GitLab.
   - Click on **"Merge Requests"** > **"New merge request"**.
   - Select `feature/add-login` as the source branch and `main` as the target branch.
   - Submit the MR for review.

### 5.2. Maintainer Workflow

1. **Reviewing the Merge Request**:

   - Navigate to **"Merge Requests"** in your project.
   - Select the pending MR (e.g., `feature/add-login`).
   - Review the changes, leave comments if necessary.

2. **Approving and Merging**:

   - After satisfactory review, approve the MR.
   - Click **"Merge"** to integrate the changes into `main`.

**Outcome**: The `main` branch remains protected, with all changes vetted through MRs and approved by you.

---

## Additional Tips

### A. Enforcing Branch Naming Conventions

Consistent branch naming helps in applying rules efficiently. Encourage collaborators to follow naming conventions like `feature/*`, `bugfix/*`, etc.

### B. Communicating Workflow Policies

Ensure that all collaborators are aware of the repository's workflow, branch protection rules, and MR approval processes to maintain smooth collaboration.

### C. Monitoring Access and Permissions

Regularly review project members and their roles to ensure that permissions align with their responsibilities. Adjust roles as necessary to maintain security.

### D. Utilizing GitLab CI/CD for Enhanced Control

Implement CI/CD pipelines to automate checks, tests, and enforce code quality before merging changes.

---

## Conclusion

By meticulously setting up branch protections, defining collaborator roles, and configuring merge request approvals, you can maintain a secure and efficient workflow in your GitLab repository. This setup ensures that critical branches like `main` and `RELEASE` remain stable and that all changes are thoroughly reviewed and approved by authorized personnel.

Following the steps outlined above will help you manage collaborator access effectively, fostering a collaborative environment while safeguarding your project's integrity.