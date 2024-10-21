
# MKPSXISO-GUI

Made with [mkpsxiso](https://github.com/MeganGrass/mkpsxiso), mkpsxiso-gui is an app for Windows that dumps and creates Sony PlayStation (1994) ISO images

Both *dumpsxiso* and *mkpsxiso* are built into the app -- mkpsxiso-gui is a standalone app and does not require any external dependencies

## Basic Features
- All features from *dumpsxiso* and *mkpsxiso*
- Windows directory and file browser support
- Drag-and-Drop support
- Basic Tooltips

#### **Windows directory and file browser support**
Instead of copying, pasting and typing out commands, *mkpsxiso-gui* utilizes Windows API/GUI to select folder locations and open/save files

#### **Drag-and-Drop**
When a Sony PlayStation (1994) ISO/BIN is "dragged and dropped" onto the app, that ISO/BIN will be automatically dumped

When a *mkpsxiso* XML is "dragged and dropped" onto the app, an ISO/BIN will be automatically created

#### **Basic Tooltips**
Every interactive button, etc., has a "*tooltip*," intended to describe each feature. Simply hover the mouse above any of these features to activate 

## Images
![Alt text](/images/mkpsxiso-gui_00.jpg?raw=true "Optional Title")
![Alt text](/images/mkpsxiso-gui_01.jpg?raw=true "Optional Title")
![Alt text](/images/mkpsxiso-gui_02.jpg?raw=true "Optional Title")

## Building/Compiling
- Any modern version of [Microsoft Visual Studio](https://visualstudio.microsoft.com)
- Clone the repo and build
```
git clone https://github.com/MeganGrass/mkpsxiso-gui.git
```

## Acknowledgements
```
DUMPSXISO 2.10 - PlayStation ISO dumping tool
2017 Meido-Tek Productions (John "Lameguy" Wilbert Villamor/Lameguy64)
2020 Phoenix (SadNES cITy)
2021-2022 Silent, Chromaryu, G4Vi, and spicyjpeg
```

```
MKPSXISO 2.10 - PlayStation ISO Image Maker
2017-2022 Meido-Tek Productions (John "Lameguy" Wilbert Villamor/Lameguy64)
2021-2022 Silent, Chromaryu, G4Vi, and spicyjpeg
```

## Changelog
Version 2.1.0.1 (October 20, 2024)
- "Output Directory," "XML Output," "image_name," and "cue_sheet" fields will now be automatically repopulated every time an ISO/BIN/XML is selected via Windows file browser
- Code cleanup and background updates
- "Dark Mode" enabled by default

Version 2.1.0.0 (October 15, 2024)
- Initial release
