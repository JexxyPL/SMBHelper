#include <iostream>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <filesystem>
struct Options {
    std::string name = "";
    std::string path = "";
    bool browsable = false;
    bool writable = false;
    bool guestok = false;
    bool readonly = true;
    std::string createmask = "";
    std::string directorymask = "";
};
bool AskYesNo(const std::string& question, bool defaultYes = true) {
    std::string prompt;
    while(true) {
        std::cout << question << (defaultYes ? "(Y/n)" : "(y/N)") << '\n';
        std::getline(std::cin,prompt);
        if(prompt.empty()) { 
            return defaultYes;
        }
        if(prompt == "Y" || prompt == "y") { 
            return true;
        }
        else if(prompt == "N" || prompt == "n") { 
            return false;
        }
        else {
            std::cerr << "Invalid choice!\n";
        }
    }
}
void DisplayMenu() {
    std::string prompt = "Y"; // default choice
    std::cout << "-----------------------------------------------\n";
    std::cout << "-----------------------------------------------\n";
    std::cout << "Welcome to Simple Samba Config Generator!   \n";
    std::cout << "Do you want to start? (Y/n)\n";
    while(true) {
        std::getline(std::cin, prompt);
        if((prompt == "Y" || prompt == "y") || prompt.empty()) {
            break;
        }
        else if(prompt == "n" || prompt == "N") { 
            exit(0);
        }
        else {
            std::cerr << "Invalid choice!\n";
        }
    }
}
void Makeconf(Options* options) {
    bool check = false;
    bool mask = false;
    bool found = false;
    int choice = 0;
    std::string filemasks[][2] = {
        {"0777", "Full access for everyone"},
        {"0666", "Read/write for everyone"},
        {"0644", "Owner read/write, others read-only"},
        {"0600", "Owner only"},
        {"0660", "Owner & group read/write"},
        {"0640", "Owner read/write, group read"}
    };
    std::string dirmasks[][2] = {
        {"0777", "Full access for everyone"},
        {"0755", "Owner full, others read/execute"},
        {"0770", "Owner & group full"},
        {"0750", "Owner full, group read/execute"},
        {"0700", "Owner only"}
    };
    std::string input = "";
    while (check == false) {
        std::cout << "Specify the name for this section of config.\n";
        while(true) {
            std::getline(std::cin, input);
            if(input.empty()) { 
                std::cerr << "Name is required! Please input a name...\n";
            }
            else {
                options->name = input;
                break;
            }
        }
        input = "";
        std::cout << "Specify the path!\n";
        while(true) {
            std::getline(std::cin,input);
            if(input.empty()) { 
                std::cerr << "PATH CANT BE EMPTY!\n";
            }
            else {
                std::cout << "Proceeding with the path ' " << input << " '\n";
                options->path = input; 
                break;
            }
        }
        input = "";
        while(mask == false) {
            std::cout << "Select file permissions (create mask)\n";
            for(int i = 0; i < 6; i++) {
                std::cout << i + 1 << ") " << filemasks[i][1] << '\n';
            }
            try {
                std::cout << "\033[1;33m7) Custom (Advanced)\033[0m\n";
                if(!(std::cin >> choice)) {
                    throw std::runtime_error("PLEASE INPUT A NUMBER");
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');   
                if(choice >= 1 && choice <= 6) {
                    options->createmask = filemasks[choice - 1][0];
                    mask = true;
                }    
                else if(choice == 7) {
                    std::cout << "Enter your own custom mask (Advanced): ";
                    while(true) {
                        std::getline(std::cin,input);
                        if(input.empty()) {
                            std::cerr << "Please input mask!\n";
                        }
                        else {
                            options->createmask = input;
                            break;
                        }
                    }
                    mask = true;
                }
                else {
                    #ifdef _WIN64 
                        system("cls");
                    #else 
                        system("clear");
                    #endif
                    std::cerr << "WRONG CHOICE, PLEASE INPUT THE CORRECT CHOICE\n";
                }
            }
            catch (const std::runtime_error& error) {
                #ifdef _WIN64 
                    system("cls");
                #else 
                    system("clear");
                #endif
                std::cerr << error.what() << '\n';
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            }
        }
        mask = false;
        input = "";
        while(mask == false) {
            std::cout << "Select folder permissions (directory mask)\n";
            for(int i = 0; i < 5; i++) {
                std::cout << i + 1 << ") " << dirmasks[i][1] << '\n';
            }
            try {
                std::cout << "\033[1;33m6) Custom (Advanced)\033[0m\n";
                if(!(std::cin >> choice)) {
                    throw std::runtime_error("PLEASE INPUT A NUMBER");
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');   
                if(choice >= 1 && choice <= 5) {
                    options->directorymask = dirmasks[choice - 1][0];
                    mask = true;
                }    
                else if(choice == 6) {
                    std::cout << "Enter your own custom mask (Advanced): ";
                    while(true) {
                        std::getline(std::cin,input);
                        if(input.empty()) {
                            std::cerr << "Please input mask!\n";
                        }
                        else {
                            options->directorymask = input;
                            break;
                        }
                    }
                    mask = true;
                }
                else {
                    #ifdef _WIN64 
                        system("cls");
                    #else 
                        system("clear");
                    #endif
                    std::cerr << "WRONG CHOICE, PLEASE INPUT THE CORRECT CHOICE\n";
                }
            }
            catch (const std::runtime_error& error) {
                #ifdef _WIN64 
                    system("cls");
                #else 
                    system("clear");
                #endif
                std::cerr << error.what() << '\n';
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            }
        }
        options->browsable = AskYesNo("Do you want this share to be browsable? ");
        options->writable = AskYesNo("Do you want this share to be writable? ");
        options->guestok = AskYesNo("Do you want this share to allow guest access? ");
        options->readonly = AskYesNo("Do you want this share to be read only? ",false);
        // Final check (info)
        std::cout << "Config name: " << options->name << '\n';
        std::cout << "Path: " << options->path << '\n';
        std::cout << "Browsable: " << (options->browsable ? "Yes" : "No") << '\n'; 
        std::cout << "Writable: " << (options->writable ? "Yes" : "No") << '\n';
        std::cout << "Guest access: " << (options->guestok ? "Yes" : "No") << '\n';
        std::cout << "Read only: " << (options->readonly ? "Yes" : "No") << '\n';
        for(int i = 0; i < 6; i++) {
            if(filemasks[i][0] == options->createmask) {
                std::cout << "File permissions: " << filemasks[i][1] << '\n';
                found = true;
                break;
            }
        }
        if(found == false) {
            std::cout << "File permissions (CUSTOM MASK): " << options->createmask << '\n';
        }
        found = false; // Setting for Folder permissions check 
        for(int i = 0; i < 5; i++) {
            if(dirmasks[i][0] == options->directorymask) {
                std::cout << "Folder permissions: " << dirmasks[i][1] << '\n';
                found = true;
                break;
            }
        }
        if(found == false) {
            std::cout << "Directory permissions (CUSTOM MASK): " << options->directorymask << '\n';
        }
        // Final check (prompt)
        if(AskYesNo("Is this config correct? ")) {
            check = true;
        }
        else {
            std::cout << "Alright lets try again!\n";
        }
    }
}
std::string Generate(Options* options) {
    std::string config = ""; 
    config += "[" + options->name + "]\n";
    config += "path = " + options->path + '\n';
    config += "browsable = " + std::string(options->browsable ? "yes" : "no") + '\n';
    config += "writable = " + std::string(options->writable ? "yes" : "no") + '\n';
    config += "guest ok = " + std::string(options->guestok ? "yes" : "no") + '\n';
    config += "read only = " + std::string(options->readonly ? "yes" : "no") + '\n';
    config += "create mask = " + options->createmask + '\n';
    config += "directory mask = " + options->directorymask + '\n';
    return config;
}
bool SambaChecker() {
    #ifdef _WIN64 
        return false;
    #endif
    std::string cmds[] = {
        "samba",
        "smbd",
        "nmbd",
        "smbclient"
    }; // idk if theres more ._.
    for(const std::string& cmd : cmds) {
        if(system(("which " + cmd + " > /dev/null 2>&1").c_str()) == 0) {
            return true;
        }
    }
    return false; 
}
bool Writeconf(const std::string& config) {
    const std::string conf = "/etc/samba/smb.conf";
    const std::string backupconf = "/etc/samba/smb.conf.bak";
    if(std::filesystem::exists(conf)) {
        try {
            std::filesystem::copy(conf,backupconf,std::filesystem::copy_options::overwrite_existing);
            std::cout << "Backup successfully created! " << backupconf << '\n';
        }
        catch(const std::filesystem::filesystem_error& nobackup) { // awww smth went wrong :(
            std::cerr << "FAILED TO BACKUP THE CONFIG FILE... EXITING TO NOT CAUSE ANY HARM: " << nobackup.what() << '\n';
            return false;
        }
    }
    std::ofstream sambaconf(conf, std::ios::app);
    if(sambaconf.is_open() == false) {
        std::cerr << "FAILED TO OPEN THE FILE TO WRITE. RUN WITH SUDO (Or the config file doesn't exist?)\n";
        return false;
    }
    sambaconf << '\n' << config << '\n';
    sambaconf.close();
    std::cout << "Saved successfully to: " << conf << '\n';
    return true;
} 
int main() {
    std::ios::sync_with_stdio(false);
    Options options;
    #ifdef _WIN64 
        system("cls");
    #else 
        system("clear");
    #endif
    DisplayMenu();
    #ifdef _WIN64 
        system("cls");
    #else 
        system("clear");
    #endif
    Makeconf(&options);
    #ifdef _WIN64 
        system("cls");
    #else 
        system("clear");
    #endif
    if(SambaChecker()) {
        std::cout << "Samba detected on this server!\n";
        std::cout << "\n\n" << "This is the generated config: \n\n" << Generate(&options);
        if(AskYesNo("Do you want to save the configuration to samba's configuration file? (smb.conf) ")) {
            if(Writeconf(Generate(&options)) == false) {
                std::cerr << "SOMETHING WENT WRONG WITH WRITING THE CONFIGURATION! I'M SORRY :(\n";
                return 1;
            }
            if(std::filesystem::exists(options.path) == false) {
                if(AskYesNo("This path doesn't exist! Do you wish to create it? ")) {
                    std::filesystem::create_directories(options.path);
                    std::cout << "Share path successfully created! Thank you for using my tool\n";
                }
            }
        }
    }
    else {
        #ifdef _WIN64 
            std::cerr << "This program is supported on Linux machines... \nConfig will still be generated BUT WILL NOT BE OUTPUTED INTO CONFIG FILE ...\n"; 
        #else 
            std::cerr << "Samba not installed! Config will still be generated BUT WILL NOT BE OUTPUTED INTO CONFIG FILE... \n If this is an error on my end then im sorry." << std::endl;
            // I will add autoinstaller, but not in the first release, as I want to push out something different than my "TO DO APP" which is basic html and js ._. 
        #endif
        std::cout << "\n\n" << "Generated config: \n" << Generate(&options); 
    }
    return 0;
}