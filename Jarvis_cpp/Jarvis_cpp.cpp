// Jarvis_cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "secrets.h"
#include "speechapi_c.h"
#include <speechapi_cxx_speech_config.h>
#include <iostream> 
#include <stdlib.h>
#include "openai.hpp"
#include <speechapi_cxx.h>




//azure namespaces
using namespace std;
using namespace Microsoft::CognitiveServices::Speech;
using namespace Microsoft::CognitiveServices::Speech::Audio;

int main()
{
    //std::cout << "initalizing speech config...\n";
    //auto speechConfig = SpeechConfig::FromSubscription(Azure::Key, Azure::Region);
    //std::cout << "initalizing audio config...\n";
    //auto audioConfig = AudioConfig::FromDefaultMicrophoneInput();
    //std::cout << "initalizing speech recognizer...\n";
    //auto speechRecognizer = SpeechRecognizer::FromConfig(speechConfig, audioConfig);

    //cout << "Speak into your microphone." << std::endl;
    //auto result = speechRecognizer->RecognizeOnceAsync().get();
    //cout << "RECOGNIZED: Text=" << result->Text << std::endl;
   
    //openai::start(); // Will use the api key provided by `OPENAI_API_KEY` environment variable
    openai::start(OpenAI::Key); // Or you can handle it yourself

    auto completion = openai::completion().create(R"({
        "model": "text-davinci-003",
        "prompt": "Say this is a test",
        "max_tokens": 7,
        "temperature": 0
    })"_json); // Using user-defined (raw) string literals
    std::cout << "Response is:\n" << completion.dump(2) << '\n';

    auto image = openai::image().create({
        { "prompt", "A cute koala playing the violin"},
        { "n", 1 },
        { "size", "512x512" }
        }); // Using initializer lists
    std::cout << "Image URL is: " << image["data"][0]["url"] << '\n';
}




// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
