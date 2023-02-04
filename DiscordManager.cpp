
void DiscordHelper::help(const std::string& chatID)
{
    // Define the JSON payload for the POST request to send the message
    web::json::value payload = web::json::value::object();
    payload["content"] = web::json::value::string("HELLO!");

    // Send a POST request to the API to send the message
    auto response = http_request.request(web::http::methods::POST, "/channels/" + channelID + "/messages", payload).get();

    // If the response status code is 200 OK, the message has been successfully sent
    if (response.status_code() == 200)
    {
        std::cout << "The message has been successfully sent." << std::endl;
    }
    // If the response status code is anything else, an error has occurred
    else
    {
        std::cout << "An error has occurred." << std::endl;
    }
}

void DiscordHelper::messageHandler()
{
    // Continuously check for new messages in the channel
    while (true)
    {
        // Send a GET request to the API to retrieve the latest messages in the channel
        auto response = http_request.request(web::http::methods::GET, "/channels/" + channelID + "/messages").get();

        // If the response status code is 200 OK, there are messages to process
        if (response.status_code() == 200)
        {
            // Parse the response body as a JSON array of messages
            auto messages = response.extract_json().get();
            for (const auto& message : messages.array())
            {
                // Check if the message starts with '!'
                if (message["content"].as_string().find("!") == 0)
                {
                    // Extract the command from the message
                    std::string command = message["content"].as_string().substr(1);

                    // Dispatch the command to the appropriate method
                    if (command == "help")
                    {
                        help(message["id"].as_string());
                    }
                }
            }
        }
        // If the response status code is anything else, an error has occurred
        else
        {
            std::cout << "An error has occurred." << std::endl;
        }
    }
}

void DiscordHelper::startMessageHandler()
{
    std::thread messageHandlerThread([this]() {
        this->messageHandler();
    });
    messageHandlerThread.detach();
}


void DiscordHelper::createChannel()
{
    // Define the JSON payload for the POST request to create the channel
    web::json::value payload = web::json::value::object();
    payload["name"] = web::json::value::string(channelName);
    payload["type"] = web::json::value::number(0);

    // Send a POST request to the API to create the channel
    auto response = http_request.request(web::http::methods::POST, "/guilds/" + GUILD + "/channels", payload).get();

    // If the response status code is 201 Created, the channel has been successfully created
    if (response.status_code() == 201)
    {
        // Extract the channel ID from the response body
        channelID = response.extract_json().get().at("id").as_string();

        std::cout << "The channel has been successfully created." << std::endl;

        // Start the message handler in a separate thread
        startMessageHandler();
    }
    // If the response status code is anything else, an error has occurred
    else
    {
        std::cout << "An error has occurred." << std::endl;
    }
}

void DiscordHelper::checkChannel()
{
    // Send a GET request to the API to retrieve information about the channel
    auto response = http_request.request(web::http::methods::GET, "/channels/" + channelID).get();

    // If the response status code is 200 OK, the channel exists
    if (response.status_code() == 200)
    {
        std::cout << "The channel exists." << std::endl;
        startMessageHandler();
    }
    // If the response status code is 404 Not Found, the channel does not exist
    else if (response.status_code() == 404)
    {
        std::cout << "The channel does not exist." << std::endl;
        createChannel();
    }
    // If the response status code is anything else, an error has occurred
    else
    {
        std::cout << "An error has occurred." << std::endl;
    }
}