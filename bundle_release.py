import os
import re
from datetime import date
import shutil
import zipfile

config = {
    "bundle_portable": True,
    "bundle_installer": True,
    "version_index": None
    # 5.0.0-20072024_{version_index}
}

tmp = {}

tmp["script_path"] = os.getcwd()

def fetch_version_to_tmp():
    global tmp
    with open(os.path.join(tmp["script_path"], "Telegram/SourceFiles/core/version.h"), "r") as f:
        data = f.read()
        tmp["version_str"] = re.search(r'AppVersionStr = "(.*?)";', data).group(1)
        tmp["version_num"] = re.search(r'AppVersion = (.*?);', data).group(1)
        is_beta = re.search(r'AppBetaVersion = (.*?);', data).group(1)
        tmp["version_beta"] = True if is_beta == "true" else False
        
def make_version_index():
    global tmp
    tmp["version_index"] = f"{tmp['version_str']}-{date.today().strftime("%d%m%Y")}{f"-beta" if tmp['version_beta'] else ""}"

def set_installer_version():
    with open(os.path.join(tmp["script_path"], "Telegram/build/setup.iss"), "r") as f:
        content = f.read()
        content = re.sub(r'#define MyAppVersionFull \".*\"', f'#define MyAppVersionFull "{tmp["version_index"]}"', content)
    
    with open(os.path.join(tmp["script_path"], "Telegram/build/setup.iss"), "w") as f:
        f.write(content)

def bundle_portable():
    global tmp

    if config["bundle_portable"]:
        print("Bundling portable version...")

        shutil.rmtree(os.path.join(tmp["script_path"], "out/Release/portable"))
        os.makedirs(os.path.join(tmp["script_path"], "out/Release/portable"))

        shutil.copyfile(
            os.path.join(tmp["script_path"], "out/Release/rabbitGram.exe"),
            os.path.join(tmp["script_path"], f"out/Release/portable/rabbitGram.exe"))
        shutil.copytree(
            os.path.join(tmp["script_path"], "out/Release/modules/"),
            os.path.join(tmp["script_path"], "out/Release/portable/modules/"))
        
        shutil.make_archive(
            f"{tmp["script_path"]}/out/Release/releases/rtgdrelease-{tmp["version_index"]}/rtgdportable-x64.{tmp["version_index"]}", 
            'zip', f"{tmp["script_path"]}/out/Release/portable")
        
def bundle_installer():
    if config["bundle_installer"]:
        print("Bundling instaler...")
        os.system(f"iscc {tmp["script_path"]}/Telegram/build/setup.iss")

fetch_version_to_tmp()
make_version_index()
set_installer_version()
bundle_portable()
bundle_installer()