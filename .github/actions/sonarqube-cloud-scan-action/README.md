<!--
SPDX-License-Identifier: Apache-2.0
SPDX-FileCopyrightText: 2024 The Linux Foundation
-->

# 🔐 SonarQube Cloud Scan

Performs a SonarQube Cloud security scan of a given repository's code base.

Uploads the results to either the cloud service or an on-premise/hosted server.

## sonarqube-cloud-scan-action

## Usage Example: Action

<!-- markdownlint-disable MD013 -->

```yaml
jobs:
  sonarqube-cloud:
    name: "SonarQube Cloud Scan"
    runs-on: ubuntu-24.04
    permissions:
      # Needed to upload the results to code-scanning dashboard
      security-events: write
      # Needed to publish results and get a badge (see publish_results below)
      id-token: write
      # Uncomment these below if installing in a private repository
      # contents: read
      # actions: read
    steps:
      - name: "SonarQube Cloud Scan"
        uses: lfit-releng-reusable-workflows/.github/actions/sonarqube-cloud-scan-action@main
        with:
          SONAR_TOKEN: ${{ secrets.SONAR_TOKEN }}
```

<!-- markdownlint-enable MD013 -->

## Usage Example: Reusable Workflow

See: <https://github.com/lfit/releng-reusable-workflows/blob/main/.github/workflows/reuse-sonarqube-cloud.yaml>

## Repository Contents and Scan Configuration

Ideally, you should provide hints to the SonarQube Cloud scan as to how your
project/repository is setup. Accurate scans need to know where your source
is located (which directory) and sometimes how to build it. For certain project
types, a wrapper script containing shell code should be provided in order to
invoke a build step or process. The build wrapper script path can be provided
to the scanning action.

Usually, you need to configure the scan by creating one of these two files:

* sonar-project.properties
* sonarcloud.properties

For details on populating them with the required information, refer to the
documentation links in the section below.

# Upstream Documentation

See: <https://github.com/SonarSource/sonarqube-scan-action>

For information on the build wrapper for C language based projects:

<https://docs.sonarsource.com/sonarqube-cloud/advanced-setup/languages/c-family/prerequisites/#using-build-wrapper>

## Inputs

<!-- markdownlint-disable MD013 -->

| Variable Name         | Required | Default     | Description                                            |
| --------------------- | -------- | ----------- | ------------------------------------------------------ |
| SONAR_TOKEN           | True     | N/A         | Mandatory authentication token to upload results       |
| SONAR_ROOT_CERT       | False    | N/A         | PEM encoded server root certificate (for HTTPS upload) |
| BUILD_WRAPPER_URL     | False    | N/A         | Download location of build wrapper/shell script        |
| BUILD_WRAPPER_OUT_DIR | False    | N/A         | Local filesystem location of build artefacts           |
| LC_ALL                | False    | en_US.UTF-8 | Locale for code base (if not covered by en_US.UTF-8)   |

<!-- markdownlint-enable MD013 -->
