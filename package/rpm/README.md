# RPM package

Maintainer: [congard](https://github.com/congard)

## Requirements

```bash
sudo dnf install rpmdevtools
sudo dnf install rpmlint # optional
```

## Steps

1. Test build:
   ```bash
   rpmbuild -D"_topdir $PWD/rpmbuild" -D'debug_package %{nil}' -bb qnvsm.spec
   ```
2. Release build (from GitHub):
   ```bash
   rpmbuild -D"_topdir $PWD/rpmbuild" -D'debug_package %{nil}' -D'build_source github' -bb qnvsm.spec
   ```
   
Linting: `rpmlint qnvsm.spec`
