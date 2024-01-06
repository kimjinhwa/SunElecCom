import shutil
import os
import subprocess
import glob

Import("env")


def copy_firmware_to_backup(*args, **kwargs):

    firmware_path = env.get("PROJECT_BUILD_DIR")
    build_path = env.get("PIOENV")
    #source_directory = r'C:\Users\STELLA\Documents\PlatformIO\Projects\Esp32SNMP\.pio\build\SNMP-DEBUG'
    source_directory = firmware_path 
    source_directory = os.path.join(source_directory,build_path );
    target_directory = os.path.abspath(os.path.join(source_directory, os.pardir))
    target_directory = os.path.abspath(os.path.join(target_directory , os.pardir))
    target_directory = os.path.abspath(os.path.join(target_directory , os.pardir))
    target_directory = os.path.join(target_directory ,'uploadFirmware')
    bin_files = glob.glob(os.path.join(source_directory,'*.bin'))
    if not os.path.exists(target_directory):
        os.makedirs(target_directory)
    
    print("source_directory" + source_directory)
    print("target_directory " + target_directory)
    for bin_file in bin_files :
        print(target_directory)
        filename = os.path.basename(bin_file)
        targat_path = os.path.join(target_directory,filename)
        shutil.copy(bin_file, target_directory)

# Call the function to copy the firmware to the backup directory
copy_firmware_to_backup()
print("복사 완료")
