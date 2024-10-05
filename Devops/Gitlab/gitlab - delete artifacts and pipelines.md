
Given this limitation, we'll need to approach this differently. Here are some steps we can take:

1. First, let's verify that your token has basic access to the project:

```powershell
$response = curl -s --request GET -H "PRIVATE-TOKEN: $env:GITLAB_SOFTOKEN" "$env:CI_API_V4_URL/projects/87"
$project = $response | ConvertFrom-Json
$project | Select-Object id, name, visibility | Format-List
```

2. If that works, let's try to list the pipelines:

```powershell
$response = curl -s --request GET -H "PRIVATE-TOKEN: $env:GITLAB_SOFTOKEN" "$env:CI_API_V4_URL/projects/87/pipelines"
$pipelines = $response | ConvertFrom-Json
$pipelines | Select-Object id, status, created_at -First 10 | Format-Table -AutoSize
```

3. Now, let's try to get information about jobs in these pipelines:

```powershell
$startPipelineId = 204
$endPipelineId = 268

for ($pipelineId = $startPipelineId; $pipelineId -le $endPipelineId; $pipelineId++) {
    Write-Host "Checking jobs for pipeline $pipelineId"
    $response = curl -s --request GET -H "PRIVATE-TOKEN: $env:GITLAB_SOFTOKEN" "$env:CI_API_V4_URL/projects/87/pipelines/$pipelineId/jobs"
    $jobs = $response | ConvertFrom-Json
    if ($jobs) {
        $jobs | Select-Object id, name, status, artifacts_file -First 5 | Format-Table -AutoSize
    }
}
```

These steps will help us understand what level of access your token has and what information we can retrieve.

If you're still unable to get the information you need, here are some recommendations:

1. Check your GitLab user permissions:
   - Log into the GitLab web interface.
   - Go to the project in question.
   - Check what role you have (Developer, Maintainer, Owner, etc.).

2. Verify your access token scope:
   - In GitLab, go to your user settings.
   - Navigate to "Access Tokens".
   - Check the scopes assigned to the token you're using.

3. If possible, create a new access token with the following scopes:
   - `api`
   - `read_repository`
   - `read_registry`

4. If you're not the project owner or don't have admin rights:
   - Contact your project owner or GitLab administrator.
   - Ask them to provide you with the storage usage information or to grant you the necessary permissions.

5. Use the GitLab web interface:
   - Navigate to your project.
   - Look for a "Usage Quotas" or similar section that might show storage information.

6. If you're using GitLab CI/CD, check the pipeline configurations:
   - Look at the `.gitlab-ci.yml` file in your project.
   - Check for any `artifacts` definitions that might be creating large artifacts.

Remember, the ability to view and manage storage information often requires elevated permissions. If you're unable to access this information with your current role, you'll need to work with your GitLab administrator or project owner to either get the information or obtain the necessary permissions.

---

Thank you for providing this detailed output. Now we can see where the artifacts are being stored. Let's analyze this information:

1. We can see that several pipelines have jobs with artifacts, particularly in the later pipelines (257 onwards).

2. The largest artifacts are in the more recent pipelines, with sizes around 67 MB.

3. To calculate the total size of artifacts, let's sum up the sizes:

```powershell
$totalSize = 0
$startPipelineId = 204
$endPipelineId = 268

for ($pipelineId = $startPipelineId; $pipelineId -le $endPipelineId; $pipelineId++) {
    $response = curl -s --request GET -H "PRIVATE-TOKEN: $env:GITLAB_SOFTOKEN" "$env:CI_API_V4_URL/projects/87/pipelines/$pipelineId/jobs"
    $jobs = $response | ConvertFrom-Json
    foreach ($job in $jobs) {
        if ($job.artifacts_file) {
            $totalSize += $job.artifacts_file.size
        }
    }
}

Write-Host "Total size of artifacts: $($totalSize / 1MB) MB"
```


4. To delete these artifacts, you can use the following script:

```powershell
for ($pipelineId = $startPipelineId; $pipelineId -le $endPipelineId; $pipelineId++) {
    $response = curl -s --request GET -H "PRIVATE-TOKEN: $env:GITLAB_SOFTOKEN" "$env:CI_API_V4_URL/projects/87/pipelines/$pipelineId/jobs"
    $jobs = $response | ConvertFrom-Json
    foreach ($job in $jobs) {
        if ($job.artifacts_file) {
            Write-Host "Deleting artifacts for job $($job.id) in pipeline $pipelineId"
            $deleteResponse = curl -s --request DELETE -H "PRIVATE-TOKEN: $env:GITLAB_SOFTOKEN" "$env:CI_API_V4_URL/projects/87/jobs/$($job.id)/artifacts"
            Write-Host "Response: $deleteResponse"
        }
    }
}
```


5. After running the deletion script, wait for a while and then use the "Recalculate repository usage" button in the GitLab UI to refresh the storage statistics.

6. If you want to keep artifacts from the most recent pipelines and only delete older ones, you can modify the script to skip the last few pipelines:

```powershell
$startPipelineId = 204
$endPipelineId = 263  # Adjust this to keep artifacts from the most recent pipelines

for ($pipelineId = $startPipelineId; $pipelineId -le $endPipelineId; $pipelineId++) {
    # ... (rest of the deletion script)
}
```


7. To prevent this issue in the future, consider:
   - Adjusting your CI/CD configuration to create smaller artifacts or to clean up artifacts after they're no longer needed.
   - Setting up artifact expiration policies in your `.gitlab-ci.yml` file.
   - Regularly running cleanup scripts to remove old artifacts.

Remember to be cautious when deleting artifacts, as they might be necessary for your development or deployment processes. Always ensure you have backups or that the artifacts are truly no longer needed before deletion.

I apologize for the confusion. It seems that the artifacts were not successfully deleted despite our attempts. This could be due to several reasons:

1. Permissions: Your token might not have the necessary permissions to delete artifacts.
2. Artifact retention policies: There might be retention policies in place preventing deletion.
3. GitLab version: Some older versions of GitLab might not support this deletion method.

Let's try a different approach:

1. First, let's check if we can erase jobs instead of just deleting artifacts:

```powershell
$startPipelineId = 204
$endPipelineId = 268

for ($pipelineId = $startPipelineId; $pipelineId -le $endPipelineId; $pipelineId++) {
    $response = curl -s --request GET -H "PRIVATE-TOKEN: $env:GITLAB_SOFTOKEN" "$env:CI_API_V4_URL/projects/87/pipelines/$pipelineId/jobs"
    $jobs = $response | ConvertFrom-Json
    foreach ($job in $jobs) {
        if ($job.artifacts_file) {
            Write-Host "Erasing job $($job.id) in pipeline $pipelineId"
            $eraseResponse = curl -s --request POST -H "PRIVATE-TOKEN: $env:GITLAB_SOFTOKEN" "$env:CI_API_V4_URL/projects/87/jobs/$($job.id)/erase"
            Write-Host "Response: $eraseResponse"
        }
    }
}
```

2. If that doesn't work, we might need to delete the entire pipeline:

```powershell
for ($pipelineId = $startPipelineId; $pipelineId -le $endPipelineId; $pipelineId++) {
    Write-Host "Deleting pipeline $pipelineId"
    $deleteResponse = curl -s --request DELETE -H "PRIVATE-TOKEN: $env:GITLAB_SOFTOKEN" "$env:CI_API_V4_URL/projects/87/pipelines/$pipelineId"
    Write-Host "Response: $deleteResponse"
}
```

Be cautious with this approach as it will delete the entire pipeline, not just the artifacts.

3. After running either of these scripts, wait for a few minutes and then recalculate the storage usage:

```powershell
$totalSize = 0
for ($pipelineId = $startPipelineId; $pipelineId -le $endPipelineId; $pipelineId++) {
    $response = curl -s --request GET -H "PRIVATE-TOKEN: $env:GITLAB_SOFTOKEN" "$env:CI_API_V4_URL/projects/87/pipelines/$pipelineId/jobs"
    $jobs = $response | ConvertFrom-Json
    foreach ($job in $jobs) {
        if ($job.artifacts_file) {
            $totalSize += $job.artifacts_file.size
        }
    }
}
Write-Host "Total size of artifacts: $($totalSize / 1MB) MB"
```

If you're still seeing the same amount of storage used, here are some additional steps to consider:

1. Check your GitLab user permissions in the web interface to ensure you have the necessary rights to delete artifacts and jobs.

2. Review any artifact retention policies that might be set at the project or group level.

3. Use the GitLab web interface to manually delete some artifacts and see if that works.

4. Contact your GitLab administrator for assistance. They might need to run cleanup tasks on the server side.

5. If possible, check the GitLab server logs for any errors that might occur during the deletion process.

Remember, changes in storage usage might not be immediately reflected in GitLab's calculations. You may need to wait for some time or trigger a manual recalculation of project storage (which typically requires admin privileges).

If none of these steps work, it would be best to consult with your GitLab administrator or GitLab support, as there might be specific configurations or issues in your GitLab instance preventing artifact deletion through the API.

---

### just for reference : my full .git-cicd.yml

```yml
# A pipeline is composed of independent jobs that run scripts, grouped into stages.
# Stages run in sequential order, but jobs within stages run in parallel.
#
# For more information, see: https://docs.gitlab.com/ee/ci/yaml/index.html#stages
#
# You can copy and paste this template into a new `.gitlab-ci.yml` file.
# You should not add this template to an existing `.gitlab-ci.yml` file by using the `include:` keyword.
#
# To contribute improvements to CI/CD templates, please follow the Development guide at:
# https://docs.gitlab.com/ee/development/cicd/templates.html

stages:          # List of stages for jobs, and their order of execution
  - build
  - test
  - pack
  - release  # New stage for release job

variables:
  PIP_CACHE_DIR: "$CI_PROJECT_DIR/.pip-cache"
  VENV_PATH: "$CI_PROJECT_DIR/venv"
  VERSION: "1.3.2"  # Global VERSION variable

# Global cache configuration
cache:
  key: ${CI_COMMIT_REF_SLUG}
  paths:
    - .pip-cache/
    - venv/
  policy: pull-push

# Python-specific cache
.python-cache:
  cache:
    key: ${CI_COMMIT_REF_SLUG}-python
    paths:
      - .pip-cache/
      - venv/
    policy: pull-push

.setup_venv_windows: &setup_venv_windows
  before_script:
    - python -m venv venv
    - .\venv\Scripts\Activate.ps1
    - python -m pip install --upgrade pip
    - pip install -r requirements.txt

.setup_venv_others: &setup_venv_others
  before_script:
    - python3 -m venv venv
    - source venv/bin/activate
    - pip3 install -r requirements.txt


######################################

build-job-windows:       # This job runs in the build stage, which runs first.
  stage: build
  extends: .python-cache
  variables:
    FLET_MAIN_FILE: "main"
  tags:
    - cicd
    - softoken
    - windows
  <<: *setup_venv_windows  # This includes the setup_venv_windows anchor
  script:
    - echo "Python build started..."
    - echo "$FLET_MAIN_FILE\without-env"
    - echo "$FLET_MAIN_FILE\with-env"
    - dir
    # - cat "$FLET_MAIN_FILE.py"
    # - cat "$env:FLET_MAIN_FILE.py"
    # - python setup.py build_ext --inplace

    # move auth.cp312-win_amd64.pyd to components folder
    # - echo "Moving auth.cp312-win_amd64.pyd to components folder..." && New-Item -ItemType Directory -Force -Path .\components | Out-Null && Move-Item -Path .\auth.cp312-win_amd64.pyd -Destination .\components\auth.cp312-win_amd64.pyd -Force && if (Test-Path .\components\auth.cp312-win_amd64.pyd) { echo "File successfully moved to components folder." } else { echo "Failed to move file. Exiting with error." ; exit 1 }
    # - Remove-Item -Path .\auth.cp312-win_amd64.pyd -Force -ErrorAction SilentlyContinue
    # package the flutter based app as a executable along with all the dependencies in the _internal folder
    - flet pack "$FLET_MAIN_FILE.py" --file-description "Soft Token" --add-data "assets;assets" --add-data "bin;bin" --add-data "components;components" --add-data "dist;dist" --add-data "./libsoftoken.dll;." --icon "icon.ico" --onedir --uac-admin
    # DEBUG:
    - echo "App dist (runtime) folder created successfully."
    # Copy the icon to the dist folder
    - Copy-Item -Path "icon.ico" -Destination "dist/$FLET_MAIN_FILE" -Force
    # Copy the bin folder to the dist folder
    - Copy-Item -Path "bin" -Destination "dist/$FLET_MAIN_FILE" -Recurse -Force
    # Copy the libsoftoken.dll to the dist folder
    - Copy-Item -Path "libsoftoken.dll" -Destination "dist/$FLET_MAIN_FILE" -Force
    # Create the destination directory if it doesn't exist, suppressing output
    - New-Item -ItemType Directory -Force -Path "dist/$FLET_MAIN_FILE/assets" | Out-Null
    # Copy assets folder contents
    - Copy-Item -Path "assets\*" -Destination "dist\$FLET_MAIN_FILE\assets" -Recurse -Force
    
    - echo "Assets folder copied to dist\$FLET_MAIN_FILE\assets"

    # DEBUG:
    # - echo "Displaying contents of dist\$FLET_MAIN_FILE folder:"
    # - Get-ChildItem -Path "dist\$FLET_MAIN_FILE" -Recurse | Select-Object FullName, Length, LastWriteTime | Format-Table -AutoSize
    # Rename the executable to SoftToken.exe
    - Rename-Item -Path "dist\$FLET_MAIN_FILE\$FLET_MAIN_FILE.exe" -NewName "softToken.exe" -Force

    # - echo "T1 First time Displaying contents of BUILD_INSTALLER3_GUI_NODRIVER\AppSourcePath folder:"
    # - Get-ChildItem -Path "BUILD_INSTALLER3_GUI_NODRIVER\AppSourcePath" -Recurse | Select-Object FullName, Length, LastWriteTime | Format-Table -AutoSize
    # DEBUG:
    # - echo "After renaming Displaying contents of dist\$FLET_MAIN_FILE folder:"
    # - Get-ChildItem -Path "dist\$FLET_MAIN_FILE" -Recurse | Select-Object FullName, Length, LastWriteTime | Format-Table -AutoSize


    # Copy contents of dist folder to BUILD_INSTALLER3_GUI_NODRIVER\AppSourcePath
    - $sourceDir = "dist\$FLET_MAIN_FILE"
    - $destDir = "BUILD_INSTALLER3_GUI_NODRIVER\AppSourcePath"
    # Create the destination directory if it doesn't exist, suppressing output
    - New-Item -ItemType Directory -Force -Path $destDir
    # Delete contents of $destDir
    - Remove-Item -Path "$destDir\*" -Recurse -Force

    # DEBUG:  SHOW CONTENTS AFTER DELETING
    - echo "T2 AFTER DELETING, Displaying contents of BUILD_INSTALLER3_GUI_NODRIVER\AppSourcePath folder:"
    - Get-ChildItem -Path "BUILD_INSTALLER3_GUI_NODRIVER\AppSourcePath" -Recurse | Select-Object FullName, Length, LastWriteTime | Format-Table -AutoSize
    # Copy new contents
    - Copy-Item -Path "$sourceDir\*" -Destination $destDir -Recurse -Force

    - echo "T3 FINAL Displaying contents of BUILD_INSTALLER3_GUI_NODRIVER\AppSourcePath folder:"
    - Get-ChildItem -Path "BUILD_INSTALLER3_GUI_NODRIVER\AppSourcePath" -Recurse | Select-Object FullName, Length, LastWriteTime | Format-Table -AutoSize
    
    - echo "Contents of "dist/$FLET_MAIN_FILE" copied to $destDir"
    - echo "Additional files copied to "dist/$FLET_MAIN_FILE" folder and main executable renamed to softToken.exe."
  rules:
    - if: $CI_COMMIT_BRANCH == "main" || $CI_COMMIT_BRANCH == "RELEASE" || $CI_COMMIT_BRANCH == "DEV"
  artifacts:
    paths:
      - "BUILD_INSTALLER3_GUI_NODRIVER/AppSourcePath/*"
      - "BUILD_INSTALLER3_GUI_NODRIVER/AppSourcePath/softToken.exe"
    expire_in: 1h  # Artifacts will be automatically deleted after 1 hourto save storage space

build-job-macos:       # This job runs in the build stage, which runs first.
  stage: build
  extends: .python-cache
  tags:
    - cicd
    - softoken
    - macos
  <<: *setup_venv_others
  script:
    - echo "Compiling the code..."
    - echo "Compile complete."
  rules:
    - if: $CI_COMMIT_BRANCH == "main"

unit-test-job-windows:
  stage: test
  extends: .python-cache
  tags:
    - cicd
    - softoken
    - windows
  <<: *setup_venv_windows
  script:
    - echo "Running unit tests on Windows..."
    - pip install pytest
    - pytest tests/test_auth.py
  rules:
    - if: $CI_COMMIT_BRANCH == "main"

unit-test-job-macos:
  stage: test
  extends: .python-cache
  tags:
    - softoken
    - macos
  <<: *setup_venv_others
  script:
    - echo "Running unit tests on macOS..."
    - pip install pytest
    # - pytest tests/test_auth.py  ## TODO: add test_auth cython version to the project components
    - echo "No tests to run on macOS"
  timeout: 10m
  allow_failure: true
  when: on_success # This job will not run if the previous job fails (default behavior but just for clarity)
  rules:
    - if: $CI_COMMIT_BRANCH == "main"

lint-test-job:   # This job also runs in the test stage.
  stage: test    # It can run at the same time as unit-test-job (in parallel).
  extends: .python-cache
  tags:
    - cicd
    - softoken
  script:
    - echo "Linting code... This will take about 10 seconds."
    - echo "This job uses the "
    - sleep 5
    - echo "No lint issues found."
  rules:
    - if: $CI_COMMIT_BRANCH == "main"

pack-job-windows:      # This job runs in the deploy stage.
  stage: pack  # It only runs when *both* jobs in the test stage complete successfully.
  tags:
    - cicd
    - softoken
    - windows
  environment: production
  script:
    - echo "Packing application..."
    - |
      echo "Executing Inno Setup Compiler..."
      & 'C:\Program Files (x86)\Inno Setup 6\ISCC.exe' .\packsoftoken.iss
    - |
      if ($IsWindows) {
        echo "Packing application for windows..."
        $sourcePath = "D:\SVN-PQ-PKI-CDAC\4_code_SoftToken\Vivek\GUI\inno-OUTPUT-setupfile\CDAC_SoftToken_Win64_gui_v$env:VERSION.exe"
        $destinationFolder = "D:\SVN-PQ-PKI-CDAC\4_code_SoftToken\Vivek\GUI\FLET-APP\toGITLAB\softToken-gui-gitlab\win-setup"
        $destinationPath = "$destinationFolder\CDAC_SoftToken_Win64_gui_v$env:VERSION.exe"
        $gitlab_runtime_destinationPath = ".\win-setup\CDAC_SoftToken_Win64_gui_v$env:VERSION.exe"
        
        echo "Current directory: $PWD"
        echo "Source path: $sourcePath"
        echo "Destination folder: $destinationFolder"
        echo "Destination path: $destinationPath"
        
        # Debug: List contents of source directory
        echo "Contents of source directory:"
        Get-ChildItem -Path (Split-Path $sourcePath) | Format-Table Name, LastWriteTime, Length -AutoSize

        # Create the destination directory if it doesn't exist
        if (-Not (Test-Path -Path $destinationFolder)) {
          New-Item -ItemType Directory -Force -Path $destinationFolder
          echo "Created destination folder: $destinationFolder"
        }
        
        # Copy the file
        Copy-Item -Path $sourcePath -Destination $destinationPath -Force -ErrorAction SilentlyContinue
        if (-not $?) {
            Write-Host "Failed to copy file. Attempting to remove existing file and retry..."
            Remove-Item -Path $destinationPath -Force -ErrorAction SilentlyContinue
            Copy-Item -Path $sourcePath -Destination $destinationPath -Force
        }

        # Copy the file to the gitlab-runner directory
        Copy-Item -Path $destinationPath -Destination $gitlab_runtime_destinationPath -Force
        if (Test-Path $destinationPath) {
          echo "File successfully copied to: $destinationPath"
        } else {
          echo "Failed to copy file to: $destinationPath"
          exit 1
        }
      } else {
        echo "This job should only run on Windows"
      }
    - echo "Application successfully packed."
  artifacts:
    paths:
      - .\win-setup\CDAC_SoftToken_Win64_gui_v$VERSION.exe
    expire_in: 1h  # Artifacts will be automatically deleted after 1 hour to save storage space
  rules:
    - if: $CI_COMMIT_BRANCH == "main" || $CI_COMMIT_BRANCH == "RELEASE" || $CI_COMMIT_BRANCH == "DEV"


pack-job-macos:      # This job runs in the deploy stage.
  stage: pack  # It only runs when *both* jobs in the test stage complete successfully.
  tags:
    - cicd
    - softoken
    - macos
  environment: production
  script:
    - echo "Packing application..."
    - |
      if [[ "$OSTYPE" == "darwin"* ]]; then
        # macOS-specific commands here
        echo "Packing for macOS"
        # Add your macOS packing commands
      else
        echo "Not a macOS environment"
      fi
    - echo "Packing completed"
  rules:
    - if: $CI_COMMIT_BRANCH == "main"


# NOTE: change the version in the packsoftoken.iss file as well as the VERSION variable in the .gitlab-ci.yml file before running this job
# Release job for windows 
release-job-windows:
  stage: release
  tags:
    - cicd
    - softoken
    - windows
  environment: production
  script:
    - |
      echo "CI_PROJECT_ID: $CI_PROJECT_ID"
      echo "CI_API_V4_URL: $CI_API_V4_URL"
      echo "CI_PROJECT_URL: $CI_PROJECT_URL"
      echo "GITLAB_TOKEN: $GITLAB_SOFTOKEN"
      echo "Project Path: $CI_PROJECT_PATH_ENCODED"
      echo "Project URL: $CI_PROJECT_URL"
      echo "Job ID: $CI_JOB_ID"
    - echo "Creating release..."
    # - mkdir release
    - |
      # Set version number
      $env:VERSION = $VERSION
      echo "Version: $env:VERSION"

      # Ensure release directory exists
      mkdir release -Force

      # Copy artifacts to release directory
      Copy-Item BUILD_INSTALLER3_GUI_NODRIVER\AppSourcePath\softToken.exe release\
      Copy-Item win-setup\CDAC_SoftToken_Win64_gui_v$env:VERSION.exe release\

      # Verify project access
      $projectResponse = curl.exe -v -s -H "PRIVATE-TOKEN: $GITLAB_SOFTOKEN" "$CI_API_V4_URL/projects/$CI_PROJECT_ID"
      echo "Project access response: $projectResponse"

      # Create the tag
      $tagBody = "{`"tag_name`": `"v$env:VERSION`", `"ref`": `"$CI_COMMIT_BRANCH`"}"
      $tagResponse = curl.exe -v -s -X POST `
        -H "PRIVATE-TOKEN: $GITLAB_SOFTOKEN" `
        -H "Content-Type: application/json" `
        -d $tagBody `
        "$CI_API_V4_URL/projects/$CI_PROJECT_ID/repository/tags"
      echo "Tag creation response: $tagResponse"

      # Create the release
      $releaseBody = @{
        name = "Soft Token GUI v$env:VERSION"
        tag_name = "v$env:VERSION"
        # tag_name = "v1.2.1"
        ref = "v$env:VERSION"
        description = "Windows release $env:VERSION"
        assets = @{
          links = @(
            @{
              name = "softToken.exe"
              url = "$CI_PROJECT_URL/-/jobs/$CI_JOB_ID/artifacts/file/release/softToken.exe"
              link_type = "package"
            },
            @{
              name = "CDAC_SoftToken_Win64_gui_v$env:VERSION.exe"
              url = "$CI_PROJECT_URL/-/jobs/$CI_JOB_ID/artifacts/file/release/CDAC_SoftToken_Win64_gui_v$env:VERSION.exe"
              link_type = "package"
            },
            @{
              name = "lcs-tool.exe"
              url = "$CI_PROJECT_URL/-/raw/$CI_COMMIT_SHA/LCS_TOOL/lcs-tool.exe"
              link_type = "other"
            },
            @{
              name = "lcstool-help.txt"
              url = "$CI_PROJECT_URL/-/raw/$CI_COMMIT_SHA/LCS_TOOL/lcstool-help.txt"
              link_type = "other"
            }
          )
        }
      } | ConvertTo-Json -Depth 4

      $releaseResponse = curl.exe -v -s -X POST `
        -H "PRIVATE-TOKEN: $GITLAB_SOFTOKEN" `
        -H "Content-Type: application/json" `
        -d $releaseBody `
        "$CI_API_V4_URL/projects/$CI_PROJECT_ID/releases"
      echo "Release creation response: $releaseResponse"

      # Check if release creation was successful
      $releaseInfo = $releaseResponse | ConvertFrom-Json
      if ($releaseInfo.tag_name -eq "v$env:VERSION") {
        echo "Release v$env:VERSION created successfully"
      } else {
        echo "Failed to create release. Response:"
        echo $releaseResponse
        exit 1
      }
  rules:
    - if: $CI_COMMIT_BRANCH == "RELEASE"
      when: always
    - when: never
  needs:
    - job: build-job-windows
    - job: pack-job-windows
  artifacts:
    paths:
      - release/*
    expire_in: never





```