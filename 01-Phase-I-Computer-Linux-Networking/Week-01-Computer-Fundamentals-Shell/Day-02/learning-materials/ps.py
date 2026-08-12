import os
import sys

def get_process_info(pid):
    status_path = f"/proc/{pid}/status"
    cmdline_path = f"/proc/{pid}/cmdline"

    if not os.path.exists(f"/proc/{pid}"):
        print(f"Error: Process with PID {pid} does not exist.")
        sys.exit(1)

    process_info = {
        "Name": None,
        "State": None,
        "PPid": None,
        "PID": pid,
        "Cmdline": None,
        "VmPeak": None,
    }

    try:
        with open(status_path, "r") as f:
            for line in f:
                parts = line.split(":",1)
                if len(parts) == 2:
                    key, value = parts[0].strip(), parts[1].strip()
                    if key == "Name":
                        process_info["Name"] = value
                    elif key == "State":
                        process_info["State"] = value
                    elif key == "Pid":
                        process_info["PID"] = value
                    elif key == "PPid":
                        process_info["PPID"] = value
                    elif key == "VmPeak":
                        process_info["VmPeak"] = value
    except PermissionError:
        print(f"Error: Permission denied when accessing {status_path}.")
        sys.exit(1)
    except FileNotFoundError:
        print(f"Error: {status_path} not found.")
        sys.exit(1)
    try:
        with open(cmdline_path, "r", encoding ="utf-8", errors="ignore") as f:
            cmdline = f.read().replace('\0', ' ').strip()
            if(cmdline):
                process_info["Cmdline"] = cmdline
            else:
                process_info["Cmdline"] = "[No command line arguments]"
    except Exception:
        process_info["Cmdline"] = "[Error reading command line]"
    print("\n" + "="*35)
    print(f"       PROCESS INSPECTOR (/proc)")
    print("="*35)
    print(f"Process Name : {process_info['Name']}")
    print(f"Process State: {process_info['State']}")
    print(f"PID          : {process_info['PID']}")
    print(f"PPID         : {process_info['PPID']}")
    print(f"Peak VMem    : {process_info['VmPeak']}")
    print(f"Command Line : {process_info['Cmdline']}")
    print("="*35 + "\n")       



if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(f"Usage: python3 {sys.argv[0]} <PID>")
        sys.exit(1)
    target_pid = sys.argv[1]
    if not target_pid.isdigit():
        print("Error: PID must be a number.")
        sys.exit(1)
    get_process_info(target_pid)