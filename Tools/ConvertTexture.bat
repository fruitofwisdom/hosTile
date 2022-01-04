@rem Takes an image and converts it to a .DDS file in place.
texconv.exe -m 1 -f BC7_UNORM -o %~dp1 -y %1
