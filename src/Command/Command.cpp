#include "Command.h"

/**
 * Unpack
 * Client input parser with regular expressions usage
 * @param data inputted string
 * @return FTP command, args
 */
pair<FTPCommandList, string> FTPCommand::Unpack(string_view data) {
    string commandBuffer(data);
    regex commandParser("^([A-Za-z]+)(?: +(.+))?");
    smatch match;

    if (!regex_match(commandBuffer, match, commandParser)) {
        throw logic_error("Unable to search readed string");
    }

    if (match.size() == 0) {
        throw logic_error("Readed command not specificed");
    }

    string command = match.str(1);
    for_each(command.begin(), command.end(), [](char &element){ element = toupper(element); });
    
    if (FTPCommandListMap.find(command) == FTPCommandListMap.end()) {
        throw logic_error("Cannot identify client command");
    }

    return { FTPCommandListMap[command], match.str(2) };
}

/**
 * Packing FTP command into string
 * @param code FTP code defined in StatusCodes
 * @param argument command argument
 * @return result string
 */
string FTPCommand::Pack(StatusCodes code, string_view argument) {
    string result = to_string((int)code);
    result += " ";
    result += argument;
    return result;
}

/**
 * Get command
 * Getting stringify command
 * @param command
 * @return optional string
 */
optional<string> FTPCommand::GetCommand(FTPCommandList command) {
    for (auto [commandStringify, reservedCommand] : FTPCommandListMap) {
        if (reservedCommand == command) {
            return commandStringify;
        }
    }
    return {};
}