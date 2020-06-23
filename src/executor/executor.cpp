//======================================
// Executor class
//======================================
//

#include "executor.h"
#include "../commands/command.h"


namespace boxer::executor {

    Executor::Executor(const command_list_t & list_of_commands, string_t temp_tar_filename = "temp_archive.tar") : all_commands(list_of_commands) {
            tar = std::make_shared<asbuzz::archiver::Tar>(temp_tar_filename);
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

        return 0;
    }
    
    auto Executor::execute() -> int {

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
                
                
            }
        }

        tar->close();

        // gzip_t gzip(tar->getName(), tar->getName()+".gz");
        gzip_t gzip = std::make_shared<asbuzz::archiver::GZipper>(tar->getName(), gzip_filename+".tar.gz");
        if(!gzip->compress()) {
            log_err("Cannot create gzip file. Tar file is in the location. You can use it.");
            return -2;
        }
        if (gzip->cleanup()) {
            log_war("Temp tar file is not removed but gzip file is OK. Resuming.");
        }

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
        return false;
    }

}