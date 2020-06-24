//======================================
// Executor class
//======================================
//

#include "executor.h"
#include "../commands/command.h"
#include <filesystem>

namespace boxer::executor {

    Executor::Executor(const command_list_t & list_of_commands, string_t temp_tar_filename, string_t working_directory):
                                                     all_commands(list_of_commands), working_dir(working_directory) {
        gzip_filename = "boxer";
        string_t tarfile = "/tmp/" + temp_tar_filename + ".tar";
        log_inf("Tar file location: " + tarfile);
        tar = std::make_shared<asbuzz::archiver::Tar>(tarfile);
    }

    auto Executor::build() -> int {

        // for (auto it = all_commands.begin(); it != all_commands.end(); ++it) {
        //     if ( (*it)->getType() == boxer::commands::ARCHIVE_NAME) {
        //         auto currentOne = (*it);
        //         all_commands.remove(*it);
        //         all_commands.push_front(currentOne);
        //         break;
        //     }
        // }

        return true;
    }
    
    auto Executor::execute() -> int {
        log_inf(" .:: Executor is Started ::.");
        bool_t errorOccurred = false;
        
        if(!tar->open()) {
            log_err("Cannot open tar file");
            return -1;
        }

        int executedCommands = 0;
        for (const command_ptr & cmd : all_commands) {
            auto command_result = cmd->execute(tar);
            if (command_result.success()) {
                executedCommands++;
                auto tmp = process_result(cmd->getType(), command_result);
                if (!tmp) {
                    errorOccurred = true;
                    break;
                }

            }
        }

        // When error occurs at any executed command.
        if (errorOccurred) {
            log_err("Exiting due to errors.");
            if (!tar->close()) {
                log_err("Tar closing error");
            }
            if(std::filesystem::remove(tar->getName())) {
                log_err("Tar removing error.");
            }
            exit(1);
        }

        if (!tar->close()) {
            log_err("Tar close error");
        }

        // gzip_t gzip(tar->getName(), tar->getName()+".gz");
        string_t gzip_location = working_dir + "/" + gzip_filename + ".tar.gz";
        gzip_t gzip = std::make_shared<asbuzz::archiver::GZipper>(tar->getName(), gzip_location);
        
        if(!gzip->compress()) {
            log_err("Cannot create gzip file. Tar file is in the location. You can use it.");
            return -2;
        }

        // if (gzip->cleanup()) {
        //     log_war("Temp tar file is not removed but gzip file is OK. Resuming.");
        // }

        return executedCommands;
    }

    auto Executor::process_result(boxer::commands::CommandType type, const boxer::commands::result::CommandResult & cmd_result) -> bool_t {
        if (cmd_result.payloadExists()) {
            switch (type)
            {
            case boxer::commands::ARCHIVE_NAME:
                gzip_filename = cmd_result.payload().at(0);
                return true;
                break;
            
            default:
                log_err("Unknown command type.");
                return false;
                break;
            }
        }
        return cmd_result.success();
    }

}