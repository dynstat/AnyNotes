# Git Commit Squashing: Comprehensive Guide

Commit squashing is an essential technique in Git for creating a cleaner, more understandable commit history. By combining multiple commits into a single one, you can simplify project history, making it easier for collaborators to follow changes and for you to manage your codebase effectively.

This guide covers:

- **What is Commit Squashing?**
- **Why Use Commit Squashing?**
- **Methods to Squash Commits**
  - **Interactive Rebase (`git rebase -i`)**
  - **Merging with `--squash`**
  - **Using `git reset` and `git commit`**
- **Usage Scenarios with Examples**
- **Before and After Git History Visualizations**
- **Best Practices**

---

## Table of Contents

1. [What is Commit Squashing?](#what-is-commit-squashing)
2. [Why Use Commit Squashing?](#why-use-commit-squashing)
3. [Methods to Squash Commits](#methods-to-squash-commits)
   - [Interactive Rebase (`git rebase -i`)](#interactive-rebase-git-rebase-i)
   - [Merging with `--squash`](#merging-with--squash)
   - [Using `git reset` and `git commit`](#using-git-reset-and-git-commit)
4. [Usage Scenarios with Examples](#usage-scenarios-with-examples)
   - [Scenario 1: Cleaning Up Feature Branch Before Merging](#scenario-1-cleaning-up-feature-branch-before-merging)
   - [Scenario 2: Combining Fixes and Enhancements](#scenario-2-combining-fixes-and-enhancements)
   - [Scenario 3: Removing WIP Commits from History](#scenario-3-removing-wip-commits-from-history)
5. [Before and After Git History Visualizations](#before-and-after-git-history-visualizations)
6. [Best Practices](#best-practices)
7. [Summary](#summary)
8. [Additional Resources](#additional-resources)

---

## What is Commit Squashing?

**Commit squashing** is the process of combining multiple Git commits into a single commit. This technique is primarily used to:

- Simplify the commit history.
- Make the history easier to navigate and understand.
- Bundle related changes together.

---

## Why Use Commit Squashing?

- **Cleaner History**: Reduces noise by eliminating minor or repetitive commits.
- **Easier Code Reviews**: Reviewers can focus on a comprehensive set of changes rather than sifting through numerous small commits.
- **Reversion Simplicity**: Easier to revert changes if needed since related changes are encapsulated in a single commit.
- **Maintaining Logical Grouping**: Ensures that each commit represents a logical unit of work.

---

## Methods to Squash Commits

There are multiple ways to squash commits in Git. The most common methods include:

1. **Interactive Rebase (`git rebase -i`)**
2. **Merging with `--squash`**
3. **Using `git reset` and `git commit`**

### Interactive Rebase (`git rebase -i`)

The most flexible and widely used method for squashing commits.

#### Commands

```bash
# Start interactive rebase for the last N commits
git rebase -i HEAD~N
```

- **`HEAD~N`**: Specifies the range of commits to include in the rebase, where `N` is the number of commits from the current `HEAD`.

#### Example

Suppose you have the following commit history:

```bash
$ git log --oneline
def456 (HEAD -> feature) Add user authentication
abc123 Add login form
789ghi Initial commit
```

You want to squash the last two commits (`def456` and `abc123`) into a single commit.

##### Commands

```bash
# Start interactive rebase for the last 2 commits
git rebase -i HEAD~2
```

This command opens an editor with a list of commits:

```
pick abc123 Add login form
pick def456 Add user authentication

# Rebase abc123..def456 onto abc123 (1 command(s))
```

Change `pick` to `s` (or `squash`) for the second commit:

```
pick abc123 Add login form
s def456 Add user authentication
```

Save and close the editor.

Git will prompt you to edit the commit message for the squashed commit. Modify it as needed and save.

##### After Rebase

```bash
$ git log --oneline
mno789 (HEAD -> feature) Add user authentication and login form
789ghi Initial commit
```

**Explanation**:
- The two commits have been combined into a single commit `mno789`.

---

### Merging with `--squash`

Another method to combine commits is by using `git merge` with the `--squash` option.

#### Commands

```bash
# Switch to the target branch
git checkout main

# Merge feature branch with squash
git merge --squash feature

# Commit the squashed changes
git commit -m "Add user authentication and login form"
```

#### Example

**Initial State**

```bash
# main branch history
$ git log --oneline
ghi789 (HEAD -> main) Initial commit

# feature branch history
$ git log --oneline
def456 Add user authentication
abc123 Add login form
789ghi Initial commit
```

**Commands**

```bash
git checkout main
git merge --squash feature
git commit -m "Add user authentication and login form"
```

**After Merge**

```bash
# main branch history
$ git log --oneline
mno789 (HEAD -> main) Add user authentication and login form
ghi789 Initial commit

# feature branch remains unchanged
$ git log --oneline
def456 Add user authentication
abc123 Add login form
789ghi Initial commit
```

**Explanation**:
- The `--squash` option combines all changes from the `feature` branch into a single commit on the `main` branch without creating a merge commit.
- The `feature` branch remains intact with its original commits.

---

### Using `git reset` and `git commit`

This method involves resetting the branch to a previous commit and then recommitting the desired changes.

#### Commands

```bash
# Reset to the commit before the ones you want to squash
git reset --soft HEAD~N

# Create a new squashed commit
git commit -m "Your squashed commit message"
```

- **`--soft`**: Keeps changes staged in the index.

#### Example

Suppose your commit history is:

```bash
$ git log --oneline
def456 (HEAD -> feature) Add user authentication
abc123 Add login form
789ghi Initial commit
```

You want to squash the last two commits into one.

##### Commands

```bash
# Reset to two commits back, keeping changes staged
git reset --soft HEAD~2

# Commit the changes as a single commit
git commit -m "Add user authentication and login form"
```

##### After Reset and Commit

```bash
$ git log --oneline
mno789 (HEAD -> feature) Add user authentication and login form
789ghi Initial commit
```

**Explanation**:
- `git reset --soft HEAD~2` moves `HEAD` back by two commits, keeping the changes staged.
- `git commit -m "..."` creates a new commit with all the staged changes.

---

## Usage Scenarios with Examples

### Scenario 1: Cleaning Up Feature Branch Before Merging

**Objective**: Combine multiple work-in-progress (WIP) commits into a single coherent commit before merging into `main`.

#### Before Squashing

```bash
# Commit history on feature branch
$ git log --oneline --graph
* def456 (HEAD -> feature) Fix typo in login form
* abc123 Add login form
* 789ghi Initial commit
```

**Commands**

```bash
# Start interactive rebase for the last 2 commits
git rebase -i HEAD~2
```

Change the second commit from `pick` to `s`.

```
pick abc123 Add login form
s def456 Fix typo in login form
```

Save and close the editor.

**After Squashing**

```bash
$ git log --oneline --graph
* mno789 (HEAD -> feature) Add login form and fix typos
* 789ghi Initial commit
```

**Explanation**:
- Combined `abc123` and `def456` into `mno789` for a cleaner history.

---

### Scenario 2: Combining Fixes and Enhancements

**Objective**: Squash fix commits into the main feature commit to maintain a linear and meaningful commit history.

#### Before Squashing

```bash
# Commit history on feature branch
$ git log --oneline --graph
* def456 (HEAD -> feature) Fix bug in user authentication
* ghi789 Add user authentication
* abc123 Add login form
* 789ghi Initial commit
```

**Commands**

```bash
# Start interactive rebase for the last 3 commits
git rebase -i HEAD~3
```

Change commits:

```
pick abc123 Add login form
pick ghi789 Add user authentication
s def456 Fix bug in user authentication
```

Save and close the editor.

**After Squashing**

```bash
$ git log --oneline --graph
* mno012 (HEAD -> feature) Add user authentication and fix related bugs
* 789ghi Initial commit
```

**Explanation**:
- Squashed `ghi789` and `def456` into `mno012`, merging enhancements and fixes into a single commit.

---

### Scenario 3: Removing WIP Commits from History

**Objective**: Eliminate work-in-progress commits that are not relevant to the final state of the feature.

#### Before Squashing

```bash
# Commit history on feature branch
$ git log --oneline --graph
* ghi789 (HEAD -> feature) WIP: Integrate OAuth
* def456 Add user authentication
* abc123 Add login form
* 789ghi Initial commit
```

**Commands**

```bash
# Start interactive rebase for the last 3 commits
git rebase -i HEAD~3
```

Change commits:

```
pick abc123 Add login form
pick def456 Add user authentication
s ghi789 WIP: Integrate OAuth
```

Save and close the editor.

**After Squashing**

```bash
$ git log --oneline --graph
* mno345 (HEAD -> feature) Add user authentication and integrate OAuth
* 789ghi Initial commit
```

**Explanation**:
- Combined the WIP commit `ghi789` with relevant commits, ensuring only meaningful changes remain in history.

---

## Before and After Git History Visualizations

### Example 1: Interactive Rebase Squash

#### Before Squashing

```bash
$ git log --oneline --graph
* def456 (HEAD -> feature) Fix typo in login form
* abc123 Add login form
* 789ghi Initial commit
```

#### Commands

```bash
git rebase -i HEAD~2
```

Update commit list:

```
pick abc123 Add login form
s def456 Fix typo in login form
```

#### After Squashing

```bash
$ git log --oneline --graph
* mno789 (HEAD -> feature) Add login form and fix typos
* 789ghi Initial commit
```

---

### Example 2: Merging with `--squash`

#### Before Squashing

```bash
# main branch
$ git log --oneline --graph
* ghi789 (HEAD -> main) Initial commit

# feature branch
$ git log --oneline --graph
* def456 (feature) Add user authentication
* abc123 Add login form
* 789ghi Initial commit
```

#### Commands

```bash
git checkout main
git merge --squash feature
git commit -m "Add user authentication and login form"
```

#### After Squashing

```bash
# main branch
$ git log --oneline --graph
* mno789 (HEAD -> main) Add user authentication and login form
* ghi789 Initial commit

# feature branch remains unchanged
$ git log --oneline --graph
* def456 (feature) Add user authentication
* abc123 Add login form
* 789ghi Initial commit
```

---

### Example 3: Using `git reset` and `git commit`

#### Before Squashing

```bash
$ git log --oneline --graph
* def456 (HEAD -> feature) Fix bug in user authentication
* abc123 Add login form
* 789ghi Initial commit
```

#### Commands

```bash
git reset --soft HEAD~2
git commit -m "Add user authentication and login form"
```

#### After Squashing

```bash
$ git log --oneline --graph
* mno012 (HEAD -> feature) Add user authentication and login form
* 789ghi Initial commit
```

---

## Best Practices

1. **Squash Before Merging to Main Branch**
   - Ensure feature branches have a clean history before merging to `main` or `master`.

2. **Use Meaningful Commit Messages**
   - Combine commits that logically belong together and write descriptive commit messages.

3. **Avoid Squashing Public Commits**
   - Rewriting history of shared branches can lead to conflicts. Prefer squashing on private feature branches.

4. **Backup Before Major Operations**
   - Create a backup branch before performing interactive rebases or resets.

   ```bash
   git branch backup-feature
   ```

5. **Communicate with Team Members**
   - Inform your team if you plan to squash commits, especially on shared branches.

6. **Review Changes Before Squashing**
   - Ensure that all changes are properly staged and intended to be combined.

   ```bash
   git status
   git diff
   ```

7. **Use `--force-with-lease` When Pushing After Squashing**
   - To safely update remote branches after rewriting history.

   ```bash
   git push --force-with-lease origin feature
   ```

8. **Test After Squashing**
   - Verify that the codebase remains stable and all tests pass after squashing commits.

---

## Summary

Commit squashing is a powerful tool in Git that helps maintain a clean and manageable project history. By combining multiple commits into a single, meaningful commit, you can:

- Simplify the commit log.
- Enhance code review processes.
- Maintain logical grouping of changes.
- Facilitate easier reversion of changes if needed.

**Key Takeaways**:

- **Interactive Rebase (`git rebase -i`)**: Offers the most flexibility for squashing commits and editing commit messages.
- **Merging with `--squash`**: Useful for combining feature branch changes into a single commit on the target branch.
- **Using `git reset` and `git commit`**: A straightforward method for local commit squashing.

**Best Practices** emphasize squashing commits on private branches, maintaining meaningful commit messages, and communicating with team members to avoid conflicts.

By integrating commit squashing into your Git workflow, you ensure a streamlined and efficient project history, enhancing collaboration and project maintenance.

---
