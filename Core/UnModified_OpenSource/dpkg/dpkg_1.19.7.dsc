-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (native)
Source: dpkg
Binary: dpkg, libdpkg-dev, dpkg-dev, libdpkg-perl, dselect
Architecture: any all
Version: 1.19.7
Maintainer: Dpkg Developers <debian-dpkg@lists.debian.org>
Uploaders: Guillem Jover <guillem@debian.org>
Homepage: https://wiki.debian.org/Teams/Dpkg
Standards-Version: 4.3.0
Vcs-Browser: https://git.dpkg.org/cgit/dpkg/dpkg.git
Vcs-Git: https://git.dpkg.org/git/dpkg/dpkg.git
Testsuite: autopkgtest
Testsuite-Triggers: autoconf, build-essential, file, pkg-config
Build-Depends: debhelper (>= 11), pkg-config, gettext (>= 0.19.7), po4a (>= 0.43), zlib1g-dev, libbz2-dev, liblzma-dev, libselinux1-dev [linux-any], libncurses-dev (>= 6.1+20180210) | libncursesw5-dev, bzip2 <!nocheck>, xz-utils <!nocheck>
Package-List:
 dpkg deb admin required arch=any essential=yes
 dpkg-dev deb utils optional arch=all
 dselect deb admin optional arch=any
 libdpkg-dev deb libdevel optional arch=any
 libdpkg-perl deb perl optional arch=all
Checksums-Sha1:
 186e350a50fed923f6a784a6d0a7183d7869ec1c 4716724 dpkg_1.19.7.tar.xz
Checksums-Sha256:
 4c27fededf620c0aa522fff1a48577ba08144445341257502e7730f2b1a296e8 4716724 dpkg_1.19.7.tar.xz
Files:
 60f57c5494e6dfa177504d47bfa0e383 4716724 dpkg_1.19.7.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEETz509DYFDBD1aWV0uXK/PqSuV6MFAlz1k48ACgkQuXK/PqSu
V6OhPhAA0nYyXE/9YcSO4Wm4mhDtnTQ1mqNC4fgOAnH/Osy7WUSCVpZa27LqcsSB
8a92Q51sq8RHbS+1oH1NoynC7yY2Ur/9QS3n+jU0udotfJd+lrmRVut9HiVrecfw
Ltnoyoz5DwjbrkSKu2c+dIeNEEqqDustvBMGsWbOO14MgAHbU8k/8zezz3wS2Acz
cptEydbBj4pWiaPZp7X861sGtE2mUXUN6amHCLOVHc1Is6cPEhF/haAiBSXt52Ie
39lnR4LkzY+s+xQUmVwtKS2QcscSJvuXlxwqRqTKVmoGLlUfZEdOrixpyq5UVBON
o5uc9qeouzFtpf5wxrAKmfzRVZAlAnt5jkQtaLMjXdNI5KgUPsMo7J7ViYPo36ez
1yKN64IosHZnyKm0y1sigrgw283nixoa10Gop1JxYC8jqzjf151e9HiwAR0RkNlT
VsEH7BU5NV7vHvH9YuTYXNWWpJJE/cTBT4Yaz8yQfCH/JUGbBSKcD8Inog6Ti/0f
VRAP+ETNuaJrR2jK+jF7v7s6ELaGpkhJuaT7EooXqPI6EUdKxac2LBUQVt2pO1W4
VQDDPLjyicG2C5JPnxgnoqiWnW14W1r9G6GceZ7j+ByfMjz4lRdHpqAU3Z96kgdh
UyidgCHn8T5jDMFnDRHyLv2/1e4koKAA7pwe1x63avJb1Wc3sXs=
=Z5Se
-----END PGP SIGNATURE-----
