# PlatformIO: "Initializing PlatformIO Core..." Stuck

If the PlatformIO sidebar stays on **"Initializing PlatformIO Core..."** and never finishes, try these steps in order.

## 1. OneDrive / cloud-synced folders (very common)

This project lives under **OneDrive**. Synced folders can lock files and slow I/O, which often causes PlatformIO Core to hang during init.

**Fix:** Clone or copy the project to a **local folder outside OneDrive**, e.g.:

- `C:\Dev\FRC2026-FuelHubCounter`
- `C:\Users\<You>\Projects\FRC2026-FuelHubCounter`

Then in Cursor: **File → Open Folder** and open that local copy. Try opening the PlatformIO tab again.

## 2. Full restart

1. Close Cursor completely (all windows).
2. Reopen Cursor and open the project folder again.
3. Open the PlatformIO tab and wait 1–2 minutes (first run can be slow).

## 3. Clear PlatformIO data and let it reinstall

PlatformIO keeps data in your user profile. Clearing it forces a clean reinstall of Core.

1. Close Cursor.
2. Delete these folders if they exist:
   - `C:\Users\<YourUsername>\.platformio`
   - `C:\.platformio`
3. Reopen Cursor and the project. Opening the PlatformIO tab will trigger a fresh Core install (may take a few minutes).

## 4. Use built-in Core and avoid duplicate installs

Multiple PlatformIO / Python installs can conflict.

1. In Cursor: **File → Preferences → Settings** (or `Ctrl+,`).
2. Search for **PlatformIO**.
3. Ensure **Use Builtin PIOCore** (or similar) is **enabled** so the extension uses its own Core.
4. If you installed PlatformIO via `pip` or standalone, uninstall it:
   - `pip uninstall platformio`
   - or `python -m pip uninstall platformio`

Then restart Cursor and try again.

## 5. Python (Windows)

PlatformIO Core depends on Python. Conflicting or missing Python can cause init to hang.

- Install the latest **Python 3** from [python.org](https://www.python.org/downloads/) and during setup check **"Add Python to PATH"**.
- If you have several Python versions, consider keeping only one (e.g. the latest 3.x) to avoid conflicts.

## 6. Test from the system terminal

To see if the problem is the IDE or Core itself:

1. Open **PowerShell** or **Cmd** (outside Cursor).
2. Run:
   ```bash
   pip install platformio
   pio --version
   ```
3. If that works, Core is fine and the hang is likely IDE/OneDrive/settings. If it fails, fix Python/install first.

## 7. Reinstall the PlatformIO extension

1. In Cursor: **Extensions** (`Ctrl+Shift+X`).
2. Find **PlatformIO IDE**, uninstall it.
3. Restart Cursor.
4. Install **PlatformIO IDE** again and restart once more.
5. Open the project (ideally from a **non–OneDrive** path) and try the PlatformIO tab again.

---

**Summary:** The most frequent cause when the project is in OneDrive is **file locking/sync**. Moving the project to a local folder (e.g. `C:\Dev\...`) and opening that folder in Cursor usually resolves "Initializing PlatformIO Core..." hanging.
