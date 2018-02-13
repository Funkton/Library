# Library
EAGLE Master Library Folder.
Cameron Nicks
cameron.nicks+layout@gmail.com

This library includes libraries, scripts, and settings.
Use at your own risk.
Any enhancements/fixes are welcome.

Notes:
Eagle shows entire folder structure in the control panel, but does not support subfolders when running from scripts and console.
If you want Eagle to use all ULPs in organized folders, add each one to the Directories settings.
I think it's best to clear all default directories in Eagle and solely use dedicated user LAYOUT folder.
The layout folder should include everything used by Eagle including all libraries, settings, and projects.
That way you can keep a full (backed up) snapshot of all Eagle resources that can easily be moved and shared.

Structure:
-Library
  - 3D
    - Components (Most models from https://www.3dcontentcentral.com/)
      - Models separated into Types.
    - ENCLOSURE
  - DRAWINGS
  - LBR
  - REFERENCE
  - SCR
    - Attributes (scripts for attribute additions to entire libraries, see notes in ulps)
    - Configure (Shortcuts and colors settings files, called from eagle.scr)
  - ULP
    - BRD (Board specific ULPs)
    - GLOBAL (brd/sch/lib ULPs)
    - LIB (Library specific ULPs)
    - SCH (Schematic specific ULPs)
