#include <cpprest/http_client.h>
#include <iostream>
#include <string>
#include <thread>

class DiscordHelper
{
    private:
        web::http::client::http_client http_request;
        std::string TOKEN;
        std::string channelID;
        std::string GUILD;
        std::string channelName;

    public:
        DiscordHelper(std::string token, std::string id, std::string guild, std::string name)
        : TOKEN(token), channelID(id), GUILD(guild), channelName(name) {}

        void connect()
        {
            web::http::client::http_client_config config;
            config.set_user_agent("DiscordHelper");
            config.set_credentials(web::http::client::credentials(TOKEN));
            http_request = web::http::client::http_client("https://discordapp.com/api/", config);
        }

        // Send a GET request to check if the channel exists
        // If the channel doesn't exist, call createChannel()
        void checkChannel();
        
        // Send a POST request to create the channel
        void createChannel();

        void DiscordHelper::startMessageHandler();

        // Continuously check for messages in the channel
        // If a message starts with "!", treat it as a command
        // and call the corresponding method based on the word after "!"        
        void messageHandler();

        // Output the message "HELLO!" to the chat with chatID
        void help();
};
