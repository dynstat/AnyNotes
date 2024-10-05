Artifacts in GitLab CI/CD pipelines are files or directories created during a job that you want to keep after the job finishes. They serve several purposes:

1. Passing data between jobs
2. Storing build outputs
3. Deploying files
4. Analyzing test results

Here's a simple example of how to use artifacts:

```yaml
build-job:
  stage: build
  script:
    - echo "Hello, World!" > output.txt
  artifacts:
    paths:
      - output.txt
    expire_in: 1 week
```

In this example:
- The job creates a file called `output.txt`
- The `artifacts` section specifies that we want to keep this file
- `expire_in: 1 week` means the artifact will be automatically deleted after one week

Practical scenarios:

1. Passing build results to a test job:

```yaml
build-job:
  stage: build
  script:
    - make build
  artifacts:
    paths:
      - build/
    expire_in: 1 hour

test-job:
  stage: test
  script:
    - test -f build/myapp || exit 1
    - ./run_tests.sh
```

Here, the `build-job` creates a `build/` directory and saves it as an artifact. The `test-job` can then access this directory to run tests on the built application.

2. Storing test reports:

```yaml
test-job:
  stage: test
  script:
    - pytest --junitxml=report.xml
  artifacts:
    reports:
      junit: report.xml
```

This job runs tests and generates a JUnit XML report, which is then stored as an artifact. GitLab will automatically parse this report and display test results in the UI.

Now, let's look at a more complex scenario:

```yaml
stages:
  - build
  - test
  - package
  - deploy

variables:
  APP_VERSION: "1.0.${CI_PIPELINE_IID}"

build-job:
  stage: build
  script:
    - echo "Building app version ${APP_VERSION}"
    - mkdir build
    - echo "App content" > build/app.txt
  artifacts:
    paths:
      - build/
    expire_in: 1 day

test-job:
  stage: test
  script:
    - test -f build/app.txt || exit 1
    - echo "Running tests..."
    - echo "Test results" > test-results.xml
  artifacts:
    reports:
      junit: test-results.xml

package-job:
  stage: package
  script:
    - echo "Packaging app..."
    - tar -czf app-${APP_VERSION}.tar.gz build/
  artifacts:
    paths:
      - app-${APP_VERSION}.tar.gz
    expire_in: 1 week

deploy-job:
  stage: deploy
  script:
    - echo "Deploying app version ${APP_VERSION}"
    - mkdir -p /deploy/
    - tar -xzf app-${APP_VERSION}.tar.gz -C /deploy/
  environment: production
```

In this complex scenario:

1. The `build-job` creates a build artifact.
2. The `test-job` uses the build artifact and generates a test report artifact.
3. The `package-job` creates a tarball artifact using the build artifact and a dynamic version number.
4. The `deploy-job` uses the packaged artifact for deployment.

This pipeline demonstrates how artifacts can be used to pass data between stages, store build outputs, save test results, and prepare deployable packages.

Some advanced features of artifacts:

1. Artifact dependencies:
```yaml
job2:
  needs:
    - job: job1
      artifacts: true
```
This ensures `job2` has access to `job1`'s artifacts.

2. Artifacts with include/exclude patterns:
```yaml
artifacts:
  paths:
    - build/**/**.txt
  exclude:
    - build/**/*.log
```

3. Exposing artifacts in merge requests:
```yaml
artifacts:
  expose_as: 'artifact 1'
```


---
