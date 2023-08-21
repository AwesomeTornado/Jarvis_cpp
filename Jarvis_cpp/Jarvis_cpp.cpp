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
#include <sapi.h>
#include <String>
#include "nlohmann/json.hpp"
#include "utf8.h"


//azure namespaces
using namespace std;
using namespace Microsoft::CognitiveServices::Speech;
using namespace Microsoft::CognitiveServices::Speech::Audio;
using json = nlohmann::json;



int main()
{
	std::cout << "initalizing speech config...\n";
	auto speechConfig = SpeechConfig::FromSubscription(Azure::Key, Azure::Region);
	std::cout << "initalizing audio config...\n";
	auto audioConfig = AudioConfig::FromDefaultMicrophoneInput();
	std::cout << "initalizing speech recognizer...\n";
	auto speechRecognizer = SpeechRecognizer::FromConfig(speechConfig, audioConfig);

	cout << "Speak into your microphone." << std::endl;
	auto result = speechRecognizer->RecognizeOnceAsync().get();
	cout << "RECOGNIZED: Text=" << result->Text << std::endl;

	openai::start(OpenAI::Key); // Or you can handle it yourself



	json request = {
		{"model", "gpt-3.5-turbo"},
		{"messages",{
		{
			{"role", "system"},
			{"content", "respond with only working powershell commands. double check your responses to ensure that it is only comprised of VALID and SHORT windows powershell commands. use no backslashes and no quotation marks."}},
		{
			{"role", "user"},
			{"content",result->Text.c_str()}
		}
		}
		},
		{"temperature", 1},
		{"max_tokens", 80},
		{"top_p", 1},
		{"frequency_penalty", 0},
		{"presence_penalty", 0}
	};


	auto completion = openai::chat().create(request);
	char* response = (char*)completion.at("choices").at(0).at("message").at("content").dump(0).c_str();
	std::cout << "Response is:\n" << response << '\n';
	string response2 = response;


	ISpVoice* pVoice = NULL;
	HRESULT hr;
	auto a = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (FAILED(a))
	{
		cout << "ERROR 404 FAILED INITIALIZING COM\n";
		exit(1);
	}
	HRESULT CoInitializeEx(LPVOID pvReserved, DWORD dwCoInit);
	hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);

	if (SUCCEEDED(hr))
	{
		vector<unsigned short> utf16line;
		string::iterator end_it = utf8::find_invalid(response2.begin(), response2.end());
		utf8::utf8to16(response2.begin(), end_it, back_inserter(utf16line));
		wstring wstr = (wchar_t*)utf16line.data();
		hr = pVoice->Speak(wstr.c_str(), 0, NULL);
		pVoice->Release();
		pVoice = NULL;
	}
	return 0;
	}
