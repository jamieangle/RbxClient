#include <iostream>
#include <windows.h>
#include <string>
#include <thread>
#include <libloaderapi.h>

using namespace std;

//Gets all of the process ids through a window title input and a process id input
void get_proc_id(const char* window_title, DWORD &process_id){
    GetWindowThreadProcessId(FindWindow(nullptr, window_title), &process_id);
}

//If there is an error
void error(const char* error_title, const char* error_message){
    MessageBox(nullptr, error_message, error_title, NULL); //Prints out error
    exit(-1); //Exits code
}

//checks if the file exists
inline bool file_exists (const string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

//Main method
int main() {
    DWORD proc_id = 0;
    char dll_path[MAX_PATH];
    const char* dll_name = "Test.dll";
    const char* window_title = "Roblox"; //Might be different, check

    if(!file_exists(dll_name)){
        error("file_exists", "File Doesn't Exist");
    }
    if(!GetFullPathName(dll_name, MAX_PATH, dll_path, nullptr)){
        error("GetFullPathName", "Failed to get full path");
    }
    get_proc_id(window_title, proc_id);
    if(proc_id == NULL){
        error("get_proc_id", "Failed to get process id");
    }
    HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, 0, proc_id);
    if(!h_process){
        error("OpenProcess", "Failed to open a handle to process");
    }
    void* allocated_memory = VirtualAllocEx(h_process, nullptr, MAX_PATH,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if(!allocated_memory){
        error("VirtualAllocEx", "Failed to allocate memory");
    }
    if(!WriteProcessMemory(h_process, allocated_memory, dll_path, MAX_PATH, nullptr)){
        error("WriteProcessMemory", "Failed to write process memory");
    }
    HANDLE h_thread = CreateRemoteThread(h_process, nullptr, 0, LPTHREAD_START_ROUTINE(LoadLibraryA), allocated_memory, NULL, nullptr);
    if(h_thread){
        error("CreateRemoteThread", "Failed to create remote thread");
    }
    CloseHandle(h_thread);
    VirtualFreeEx(h_process, allocated_memory, 0, MEM_RELEASE);
    MessageBox(nullptr, "Successfully Injected!", "Success", 0);

}
