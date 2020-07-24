# SnowHaze Windows VPN Client


SnowHaze is on a mission to protect your online privacy. Hide your true IP address, encrypt your traffic, and change your virtual location with SnowHaze VPN. Our Zero-Knowledge Auth protocol uses asymmetric encryption and random tokens to provide the first anonymous VPN authorization. From registration, over payment, up to usage, every step is designed for privacy. Use SnowHaze VPN on a Windows machine with this VPN client.

SnowHaze also offers other products for privacy protection such as the SnowHaze iOS browser & VPN client and the SnowHaze Firefox extension. More details about SnowHaze and Zero-Knowledge Auth can be found on the [SnowHaze Website](https://snowhaze.com/).

Get the SnowHaze Firefox extension for free from [Mozilla](https://addons.mozilla.org/en-US/firefox/addon/snowhaze/).

Get SnowHaze on iOS for free from the [App Store](https://snowhaze.com/download).

## License

This client is licensed under the GPL v3 license.

Disclaimer: The GPL license is *not* a free license and GPL licensed software is *not* free software. The GPL license restricts your rights to use software heavily. It is designed specifically to be incompatible with many other licenses and because of this we are bound to use the GPL license. Since the GPL license confines you to the GPL ecosystem, it contradicts the very essence of free software and thus we do not endorse it.

## Getting Started

The easiest way to get the SnowHaze VPN client is to download it from [our website](https://snowhaze.com/en/download.html#get-vpn-client).

Alternatively, you can also install it directly from source, by following the steps outlined below:

1. Clone the repository

2. Add the libsodium-23.dll to the just cloned repo (the libsodium-23.dll must be in the same folder as the SnowHazeVPN.pro file)

3. Open the SnowHazeVPN.pro file in the Qt creator

4. If this is the first time you open the `SnowHazeVPN` project Qt will ask you to configure your project. Make sure that you use Qt 5.15.0 MinGW 64-bit kit to build the project.

5. In the side-bar verify again that your build kit is Qt 5.15.0 MinGW 64-bit and that you build the `release`. Now, click on the run button. This will create a folder with the name `build-SnowHazeVPN-Desktop_Qt_5_15_0_MinGW_64_bit-Release` which will be located in the parent directory of `SnowHazeVPN.pro` file.

6. In this Build-directory (`build-SnowHazeVPN-Desktop_Qt_5_15_0_MinGW_64_bit-Release`) you will find a `release` folder and a `debug` folder. To both of these folders copy the following `.dll` files:

	- D3Dcompiler_47.dll
	- libcrypto-1_1-x64.dll
	- libEGL.dll
	- libgcc_s_dw2-1.dll
	- libgcc_s_seh-1.dll
	- libGLESv2.dll
	- libsodium-23.dll
	- libssl-1_1-x64.dll
	- libstdc++-6.dll
	- libwinpthread-1.dll
	- opengl32sw.dll
	- Qt5Core.dll
	- Qt5Gui.dll
	- Qt5Network.dll
	- Qt5Svg.dll
	- Qt5Widgets.dll

7. Add to the same two folders (the `debug` and the `release` folder in your Build-directory) the following folder structure:
```bash
binaries
├── ovpn
├── python
├── sgxVerification
├── ssl
```

8.	Fetch the client's dependencies

	- Copy into `binaries/ovpn/` the openvpn-gui which you can download form the [OpenVPN download page](https://openvpn.net/community-downloads/).
	- Copy into `binaries/python/` python version 3.8 or higher which you can download form the [Python download page](https://www.python.org/downloads/windows/).
	- Clone into `binaries/sgxVerification/` the the [zka-sgx repository](https://snowhaze.com/sgx).
	- Copy into `binaries/ssl/` a 64-bit version for Windows which you can find [here](https://wiki.openssl.org/index.php/Binaries).

9. Open the Qt 5.15.0 MinGW 64-bit terminal and navigate to the location where the `SnowHazeVPN.pro` file is stored using `cd C:\...\desktop-vpn-client`.
	Run the command:
	`lrelease SnowHazeVPN.pro`

10. Build again the `release` in the Qt Creator as before.

11.	Navigate in the terminal into the `...\build-SnowHazeVPN-Desktop_Qt_5_15_0_MinGW_64_bit-Release\release\` folder.
	Run the command:
	`windeployqt "SnowHaze VPN.exe"`

12.	Close the Qt 5.15.0 MinGw 64-bit terminal and click in the `...\build-SnowHazeVPN-Desktop_Qt_5_15_0_MinGW_64_bit-Release\release\` folder on `SnowHaze VPN.exe` to start the SnowHaze VPN client.

## Versioning

This is not our working repository and we only push versions to this repository that will be released.


## Contributing

Please get in touch with us if you would like to contribute to any of our projects. We would love to have you on board with us! As this is not our working repository, we cannot accept pull-requests on this repository.

## Support

Do not hesitate to [contact us](https://snowhaze.com/en/support-contact.html) if you have any questions.


## Authors

SnowHaze was created by Illotros GmbH, all rights reserved.
