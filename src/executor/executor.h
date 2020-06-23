//======================================
// Executor class
//======================================
//
#pragma once
#include "../defs.h"
#include "../commands/command.h"
#include "../archiver/tarballer.h"
#include "../archiver/gzipper.h"
#include "../commands/result/command_result.h"

namespace boxer::executor {
    class Executor {
        private:
        string_t gzip_filename;        
        command_list_t all_commands;
        tar_t tar;
        
        auto process_result(boxer::commands::CommandType type, const boxer::commands::result::CommandResult & cmd_result) -> bool_t;

        public:
        // The tar_filename is not actual file name, it will be set via ARCHIVE_NAME command at the end.
        // temp_tar_filename has default value.
        Executor(const command_list_t & list_of_commands, string_t temp_tar_filename);

        ~Executor() { log_inf("~Executor"); }

        auto execute() -> int;
        auto build() -> int;
    };
}