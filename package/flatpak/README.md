# Flatpak package

Maintainer: [congard](https://github.com/congard)

## Requirements

1. `flatpak`
2. `flatpak-builder`

## Dependencies

1. `org.kde.Platform//6.8`
2. `org.kde.Sdk//6.8`

## Steps

```bash
# build & install
flatpak-builder --user --install --force-clean build-dir io.github.congard.qnvsm.yml

# run
flatpak run io.github.congard.qnvsm
```

## Useful commands

```bash
# put the app to the local repository
flatpak-builder --repo=repo --force-clean build-dir io.github.congard.qnvsm.yml

# create bundle
flatpak build-bundle repo qnvsm.flatpak io.github.congard.qnvsm
```
