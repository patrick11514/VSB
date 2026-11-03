#define _WIN32_WINNT 0x0600
#include <windows.h>
#include <winternl.h>
#include <shlobj.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define SMBIOS_SIGNATURE ('R' | ('S' << 8) | ('M' << 16) | ('B' << 24))

// Known VM / Hypervisor vendor keywords (case-insensitive comparison)
static const char *VM_KEYWORDS[] = {
    "VMWARE",
    "VIRTUALBOX",
    "VBOX",
    "INNOTEK",
    "QEMU",
    "BOCHS",
    "RED HAT",
    "KVM",
    "XEN",
    "HYPER-V",
    NULL
};

// Helper: case-insensitive substring search
static bool contains_ignore_case(const char *haystack, size_t haystack_len, const char *needle) {
    if (!haystack || !needle || haystack_len == 0) return false;
    size_t needle_len = strlen(needle);
    if (needle_len > haystack_len) return false;

    for (size_t i = 0; i <= haystack_len - needle_len; i++) {
        bool match = true;
        for (size_t j = 0; j < needle_len; j++) {
            if (toupper((unsigned char)haystack[i + j]) != toupper((unsigned char)needle[j])) {
                match = false;
                break;
            }
        }
        if (match) return true;
    }
    return false;
}

// ----------------------------------------------------------------------
// 1. ANTI-DEBUGGING TECHNIQUES
// ----------------------------------------------------------------------

// Technique 1a: Win32 API IsDebuggerPresent()
bool check_api_is_debugger_present(void) {
    return IsDebuggerPresent() ? true : false;
}

// Technique 1b: Direct PEB (Process Environment Block) BeingDebugged flag
bool check_peb_being_debugged(void) {
#if defined(_WIN64)
    PPEB pPeb = (PPEB)__readgsqword(0x60);
#elif defined(_WIN32)
    PPEB pPeb = (PPEB)__readfsdword(0x30);
#else
    PPEB pPeb = NULL;
#endif
    if (pPeb != NULL) {
        return (pPeb->BeingDebugged == 1);
    }
    return false;
}

// Technique 1c: CheckRemoteDebuggerPresent()
bool check_remote_debugger(void) {
    BOOL isDebuggerPresent = FALSE;
    if (CheckRemoteDebuggerPresent(GetCurrentProcess(), &isDebuggerPresent)) {
        return isDebuggerPresent ? true : false;
    }
    return false;
}

// Comprehensive debugger check
bool is_debugged(void) {
    bool dbg_api = check_api_is_debugger_present();
    bool dbg_peb = check_peb_being_debugged();
    bool dbg_rem = check_remote_debugger();

    printf("[*] Running Anti-Debug Checks:\n");
    printf("    |-- IsDebuggerPresent() API       : %s\n", dbg_api ? "[!] DETECTED" : "[+] Clear");
    printf("    |-- PEB->BeingDebugged Flag       : %s\n", dbg_peb ? "[!] DETECTED" : "[+] Clear");
    printf("    |-- CheckRemoteDebuggerPresent()  : %s\n", dbg_rem ? "[!] DETECTED" : "[+] Clear");

    return (dbg_api || dbg_peb || dbg_rem);
}

// ----------------------------------------------------------------------
// 2. ANTI-VM / HYPERVISOR DETECTION (BIOS / SMBIOS / DMI FIRMWARE)
// ----------------------------------------------------------------------

// Technique 2a: GetSystemFirmwareTable ('RSMB') SMBIOS / DMI table inspection
bool check_smbios_firmware(char *detected_vendor, size_t max_vendor_len) {
    UINT size = GetSystemFirmwareTable(SMBIOS_SIGNATURE, 0, NULL, 0);
    if (size == 0) {
        return false;
    }

    BYTE *buffer = (BYTE *)malloc(size);
    if (!buffer) {
        return false;
    }

    if (GetSystemFirmwareTable(SMBIOS_SIGNATURE, 0, buffer, size) == 0) {
        free(buffer);
        return false;
    }

    bool detected = false;
    for (int k = 0; VM_KEYWORDS[k] != NULL; k++) {
        if (contains_ignore_case((const char *)buffer, size, VM_KEYWORDS[k])) {
            if (detected_vendor && max_vendor_len > 0) {
                snprintf(detected_vendor, max_vendor_len, "SMBIOS string match '%s'", VM_KEYWORDS[k]);
            }
            detected = true;
            break;
        }
    }

    free(buffer);
    return detected;
}

// Technique 2b: Windows Registry BIOS inspection (HARDWARE\DESCRIPTION\System\BIOS)
bool check_registry_bios(char *detected_vendor, size_t max_vendor_len) {
    HKEY hKey;
    const char *subKey = "HARDWARE\\DESCRIPTION\\System\\BIOS";
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, subKey, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        return false;
    }

    const char *value_names[] = {
        "SystemManufacturer",
        "SystemProductName",
        "BIOSVendor",
        "BIOSVersion",
        "BaseBoardManufacturer",
        "BaseBoardProduct",
        NULL
    };

    char reg_data[512];
    DWORD data_size;
    DWORD type;
    bool detected = false;

    for (int i = 0; value_names[i] != NULL; i++) {
        data_size = sizeof(reg_data) - 1;
        if (RegQueryValueExA(hKey, value_names[i], NULL, &type, (LPBYTE)reg_data, &data_size) == ERROR_SUCCESS) {
            reg_data[data_size] = '\0';
            for (int k = 0; VM_KEYWORDS[k] != NULL; k++) {
                if (contains_ignore_case(reg_data, strlen(reg_data), VM_KEYWORDS[k])) {
                    if (detected_vendor && max_vendor_len > 0) {
                        snprintf(detected_vendor, max_vendor_len, "Registry [%s: '%s']", value_names[i], reg_data);
                    }
                    detected = true;
                    break;
                }
            }
            if (detected) break;
        }
    }

    RegCloseKey(hKey);
    return detected;
}

// Comprehensive VM check
bool is_virtual_machine(void) {
    char smbios_info[256] = {0};
    char reg_info[256] = {0};

    bool vm_smbios = check_smbios_firmware(smbios_info, sizeof(smbios_info));
    bool vm_reg = check_registry_bios(reg_info, sizeof(reg_info));

    printf("[*] Running Anti-VM (BIOS / Firmware) Checks:\n");
    printf("    |-- SMBIOS Firmware Table (RSMB)  : %s%s%s\n",
           vm_smbios ? "[!] DETECTED (" : "[+] Clear",
           vm_smbios ? smbios_info : "",
           vm_smbios ? ")" : "");
    printf("    |-- Registry System/BIOS Data     : %s%s%s\n",
           vm_reg ? "[!] DETECTED (" : "[+] Clear",
           vm_reg ? reg_info : "",
           vm_reg ? ")" : "");

    return (vm_smbios || vm_reg);
}

// ----------------------------------------------------------------------
// 3. BEHAVIOR MODES (TWO DISTINCT OPERATING MODES)
// ----------------------------------------------------------------------

// Determine destination file paths (Desktop and local directory fallback)
static void get_target_paths(const char *filename, char *desktop_path, size_t desktop_len, char *local_path, size_t local_len) {
    // 1. Local working directory path
    char current_dir[MAX_PATH] = {0};
    GetCurrentDirectoryA(MAX_PATH, current_dir);
    snprintf(local_path, local_len, "%s\\%s", current_dir, filename);

    // 2. Desktop path
    char desktop_dir[MAX_PATH] = {0};
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, desktop_dir))) {
        snprintf(desktop_path, desktop_len, "%s\\%s", desktop_dir, filename);
    } else {
        DWORD env_len = GetEnvironmentVariableA("USERPROFILE", desktop_dir, MAX_PATH);
        if (env_len > 0 && env_len < MAX_PATH) {
            snprintf(desktop_path, desktop_len, "%s\\Desktop\\%s", desktop_dir, filename);
        } else {
            strncpy(desktop_path, local_path, desktop_len);
        }
    }
}

static void write_file(const char *path, const char *content) {
    FILE *f = fopen(path, "w");
    if (f) {
        fputs(content, f);
        fclose(f);
        printf("[+] File successfully created at: %s\n", path);
    } else {
        printf("[-] Warning: Failed to write file at: %s\n", path);
    }
}

// Mode A: Running in VM or under Debugger (Harmless, benign evasion mode)
void kind_way(bool debugged, bool virtual_env) {
    printf("\n[!] >>> SELECTED MODE: BENIGN / EVASION MODE (kind_way) <<<\n");
    printf("[!] Analysis environment detected (Debugger: %s, VM: %s).\n",
           debugged ? "YES" : "NO",
           virtual_env ? "YES" : "NO");
    printf("[!] Suppressing payload. Operating safely.\n");

    char desktop_file[MAX_PATH * 2] = {0};
    char local_file[MAX_PATH * 2] = {0};
    get_target_paths("kind.txt", desktop_file, sizeof(desktop_file), local_file, sizeof(local_file));

    char report[1024];
    snprintf(report, sizeof(report),
        "====================================================\n"
        "PVBPS - Anti-Analysis Triggered: BENIGN MODE\n"
        "====================================================\n"
        "Debugger Detected     : %s\n"
        "VM / Sandbox Detected : %s\n"
        "Action: Payload suppressed to prevent dynamic analysis.\n",
        debugged ? "TRUE" : "FALSE",
        virtual_env ? "TRUE" : "FALSE"
    );

    // Drop to Desktop
    write_file(desktop_file, report);
    // Also drop to local directory if different from Desktop
    if (strcmp(desktop_file, local_file) != 0) {
        write_file(local_file, report);
    }
}

// Mode B: Running on Real Physical Target (Simulated malicious payload)
void malicious_way(void) {
    printf("\n[+] >>> SELECTED MODE: ACTIVE MALICIOUS PAYLOAD MODE (malicious_way) <<<\n");
    printf("[+] Environment verified as physical hardware with no active debuggers.\n");
    printf("[+] Executing simulated payload...\n");

    char desktop_file[MAX_PATH * 2] = {0};
    char local_file[MAX_PATH * 2] = {0};
    get_target_paths("malicious.txt", desktop_file, sizeof(desktop_file), local_file, sizeof(local_file));

    const char *payload_text =
        "====================================================\n"
        "PVBPS - Anti-Analysis Bypassed: MALICIOUS PAYLOAD\n"
        "====================================================\n"
        "Status: Target confirmed as genuine physical system.\n"
        "Payload: Simulated dropper activated.\n"
        "Location: Dropped on Desktop and local directory.\n"
        "Action: Simulated exfiltration and persistence established.\n";

    // Drop to Desktop
    write_file(desktop_file, payload_text);
    // Also drop to local directory if different from Desktop
    if (strcmp(desktop_file, local_file) != 0) {
        write_file(local_file, payload_text);
    }
}

// ----------------------------------------------------------------------
// MAIN ENTRY POINT
// ----------------------------------------------------------------------
int main(void) {
    printf("========================================================\n");
    printf("  PVBPS Lab 4 - Anti-Debug and Anti-VM Verification\n");
    printf("========================================================\n\n");

    bool debugged = is_debugged();
    printf("\n");
    bool virtual_env = is_virtual_machine();
    printf("\n");

    if (debugged || virtual_env) {
        kind_way(debugged, virtual_env);
    } else {
        malicious_way();
    }

    printf("\n========================================================\n");
    printf("  Execution completed.\n");
    printf("========================================================\n");
    return 0;
}