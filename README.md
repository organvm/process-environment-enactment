# Process environment enactment

A small, public conformance harness for native process-environment carriers:

- POSIX `execve(..., envp)` behavior on GitHub-hosted Ubuntu and macOS runners.
- Windows `CreateProcessW(..., lpEnvironment, ...)` behavior on a GitHub-hosted Windows runner.

## Status

The workflow is intentionally manual. Earlier staging attempts failed before runner assignment, so they did not validate the harness and produced no artifacts. This clean public projection is the hosted-runner canary. Until it completes successfully, no cross-platform result is claimed; after dispatch, its current run is the source of truth.

[View workflow runs](https://github.com/organvm/process-environment-enactment/actions/workflows/enact.yml)

## What it checks

The POSIX harness exercises ordering, duplicate entries, empty names, entries without `=`, embedded-NUL truncation, and replicate/append/omit/replace operations. The Windows harness exercises inherited and explicit environment blocks, including append/omit/replace cases.

## Run it

Open **Actions → Enact process environment → Run workflow**. A successful run uploads short-lived text artifacts containing the observed outputs and logs their SHA-256 digests.

## License

[MIT](LICENSE)
