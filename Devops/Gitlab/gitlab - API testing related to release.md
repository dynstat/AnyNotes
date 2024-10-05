# GitLab CI/CD Release Automation

This guide outlines the steps to automate the creation of tags and releases in GitLab using PowerShell scripts and `curl` commands. The process involves setting up environment variables, creating tags, and publishing releases with associated assets.

## Table of Contents

1. [Environment Setup](#environment-setup)
2. [Retrieve Project Information](#retrieve-project-information)
3. [Create a Tag](#create-a-tag)
4. [Create a Release](#create-a-release)
5. [Update Release with Assets](#update-release-with-assets)

---

## Environment Setup

First, set up the necessary environment variables required for interacting with the GitLab API.

```powershell
# Set GitLab Private Token
$env:GITLAB_SOFTOKEN = "glt-qzfasdfasdfu7_Lu"

# GitLab API Base URL
$env:CI_API_V4_URL = "https://gitlab01.dcservices.in/api/v4"

# GitLab Project URL
$env:CI_PROJECT_URL = "https://gitlab01.dcservices.in/345320/softoken-gui"

# CI Job ID (Replace with actual job ID)
$env:CI_JOB_ID = "some_job_id"

# Version Number (Update as needed)
$env:VERSION = "1.2.0"
```

### Explanation

- **$GITLAB_SOFTOKEN**: Your private token for authenticating with the GitLab API.
- **$CI_API_V4_URL**: The base URL for GitLab's API v4.
- **$CI_PROJECT_URL**: The URL of your GitLab project.
- **$CI_JOB_ID**: The ID of the CI job that created the artifacts.
- **$VERSION**: The version number for the release.

---

## Retrieve Project Information

Before performing operations, verify access to the specific GitLab project.

```powershell
# Retrieve project information using GitLab API
curl.exe -v -H "PRIVATE-TOKEN: $env:GITLAB_SOFTOKEN" "$env:CI_API_V4_URL/projects/345320%2Fsoftoken-gui"
```

### Explanation

- **curl.exe**: Executes an HTTP request.
- **-H "PRIVATE-TOKEN: $env:GITLAB_SOFTOKEN"**: Authenticates the request.
- **"$env:CI_API_V4_URL/projects/345320%2Fsoftoken-gui"**: API endpoint for the specified project.

---

## Create a Tag

Create a new Git tag referencing the `RELEASE` branch.

```powershell
# Create a new tag in the repository
curl.exe -v -X POST `
   -H "PRIVATE-TOKEN: $env:GITLAB_SOFTOKEN" `
   -H "Content-Type: application/json" `
   -d "{`"tag_name`": `"v$env:VERSION`", `"ref`": `"RELEASE`"}" `
   "$env:CI_API_V4_URL/projects/87/repository/tags"
```

### Explanation

- **-X POST**: Specifies a POST request to create a new resource.
- **-H "Content-Type: application/json"**: Sets the content type to JSON.
- **-d "{`"tag_name`": `"v$env:VERSION`", `"ref`": `"RELEASE`"}"**: JSON payload defining the tag name and the reference branch (`RELEASE`).
- **"$env:CI_API_V4_URL/projects/87/repository/tags"**: API endpoint to create a tag in project ID `87`.

---

## Create a Release

Publish a release associated with the newly created tag.

```powershell
# Create a new release in GitLab
$body = @{
   name = "Test Release $env:VERSION"
   tag_name = "v$env:VERSION"
   ref = "v$env:VERSION"
   description = "Testing automated release using CMD Rest APIs"
} | ConvertTo-Json

curl.exe -v -X POST `
   -H "PRIVATE-TOKEN: $env:GITLAB_SOFTOKEN" `
   -H "Content-Type: application/json" `
   -d $body `
   "$env:CI_API_V4_URL/projects/87/releases"
```

### Explanation

- **$body**: Constructs a JSON object with release details.
  - **name**: The name of the release.
  - **tag_name**: The associated tag.
  - **ref**: The reference commit or tag.
  - **description**: A brief description of the release.
- **ConvertTo-Json**: Converts the PowerShell object to a JSON string.
- **"$env:CI_API_V4_URL/projects/87/releases"**: API endpoint to create a release in project ID `87`.

---

## Update Release with Assets

Enhance the release by attaching executable artifacts.

```powershell
# Update the release with associated assets
$env:VERSION = "1.2.1"  # Update to your actual version number
$env:CI_JOB_ID = "1234"  # Update to the actual job ID that created the artifacts

$body = @{
    name = "Release $env:VERSION"
    tag_name = "v$env:VERSION"
    ref = "main"  # Alternatively, specify the appropriate branch or tag
    description = "Release including softToken.exe and CDAC_SoftToken_Win64_gui"
    assets = @{
        links = @(
            @{
                name = "softToken.exe"
                url = "$env:CI_PROJECT_URL/-/jobs/$env:CI_JOB_ID/artifacts/file/release/softToken.exe"
            },
            @{
                name = "CDAC_SoftToken_Win64_gui_v$env:VERSION.exe"
                url = "$env:CI_PROJECT_URL/-/jobs/$env:CI_JOB_ID/artifacts/file/release/CDAC_SoftToken_Win64_gui_v$env:VERSION.exe"
            }
        )
    }
} | ConvertTo-Json -Depth 4

curl.exe -v -X POST `
    -H "PRIVATE-TOKEN: $env:GITLAB_SOFTOKEN" `
    -H "Content-Type: application/json" `
    -d $body `
    "$env:CI_API_V4_URL/projects/87/releases"
```

### Explanation

- **$env:VERSION**: Update the version number as needed.
- **$env:CI_JOB_ID**: Update with the actual CI job ID that generated the artifacts.
- **$body**: Constructs a JSON object with release details and asset links.
  - **assets.links**: An array of assets to attach to the release.
    - **name**: The display name of the asset.
    - **url**: The direct URL to the artifact.
- **ConvertTo-Json -Depth 4**: Converts the PowerShell object to a JSON string with sufficient depth for nested objects.
- **"$env:CI_API_V4_URL/projects/87/releases"**: API endpoint to update the release with assets in project ID `87`.

---

## Summary

By organizing your release automation scripts into separate, well-documented PowerShell files, you enhance maintainability and clarity. Each script performs a distinct function:

1. **Environment Setup**: Initializes necessary variables.
2. **Retrieve Project Information**: Confirms access to the project.
3. **Create a Tag**: Tags the repository at a specific reference.
4. **Create a Release**: Publishes the release on GitLab.
5. **Update Release with Assets**: Attaches build artifacts to the release.

Ensure that you replace placeholder values like `"some_job_id"` and version numbers with actual data relevant to your project.