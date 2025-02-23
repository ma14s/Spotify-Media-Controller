#include <iostream>
#include <Windows.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/windows.foundation.collections.h>
#include <winrt/Windows.Media.Control.h>


using namespace winrt;
using namespace Windows::Media::Control;

constexpr std::wstring_view SpotifyAppID = L"SpotifyAB.SpotifyMusic_zpdnekdrzrea0!Spotify"; 



enum MediaSate
{
    Pause,Start,Next,Previous
};


static bool ChangeMediaState(const std::wstring_view SourceAppUserModelId, const MediaSate state)
{

   const auto sessionManager = GlobalSystemMediaTransportControlsSessionManager::RequestAsync().get();

    if (!sessionManager)
    {
        OutputDebugStringA("Falied To Get sessionManager");
        return false;
    }

    const auto Sessions = sessionManager.GetSessions(); 
    GlobalSystemMediaTransportControlsSession TargtedSession{ nullptr };

    for (const auto&& session : Sessions) 
    {  
        if (session.SourceAppUserModelId() == SourceAppUserModelId) 
        {
            TargtedSession = session; 
            break;  
        } 

    }

    if (!TargtedSession)
    {
        OutputDebugStringA("Falied To Get TargtedSession");
        return false;
    }

    switch (state)
    {
    case Pause:
        return TargtedSession.TryPauseAsync().get();
        break;
    case Start:
        return TargtedSession.TryPlayAsync().get();
        break;
    case Next:
        return TargtedSession.TrySkipNextAsync().get(); 
        break;
    case Previous:
        return TargtedSession.TrySkipPreviousAsync().get();
        break;
    default:
        return false;
        break;
    }
}


void GetMediaInfo() 
{
    auto sessionManager = GlobalSystemMediaTransportControlsSessionManager::RequestAsync().get();

    if (!sessionManager)
    {
        return;
    }

    const auto Sessions =  sessionManager.GetSessions();

    hstring SourceAppName;

    for (const auto& session : Sessions) 
    {
        SourceAppName = session.SourceAppUserModelId();

        if (SourceAppName == L"SpotifyAB.SpotifyMusic_zpdnekdrzrea0!Spotify")
        {

        }

    }



}

int main() 
{
    try
    {
        init_apartment();

        while (true)
        {
            if (GetAsyncKeyState(VK_ESCAPE) & 0X8000)
                break;
            
            if ((GetAsyncKeyState(VK_LSHIFT) & 0x8000) && (GetAsyncKeyState('A') & 0x8000))
            {
                if (ChangeMediaState(SpotifyAppID, MediaSate::Start))   
                    std::wcout << L"Successfully updated media state to: Start" << std::endl;
                else
                    std::wcout << L"Failed to update media state to: Start" << std::endl;

                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }

            if ((GetAsyncKeyState(VK_LSHIFT) & 0x8000) && (GetAsyncKeyState('S') & 0x8000))
            {
                if (ChangeMediaState(SpotifyAppID, MediaSate::Pause))
                    std::wcout << L"Successfully updated media state to: Pause" << std::endl;
                else
                    std::wcout << L"Failed to update media state to: Pause" << std::endl;

                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }

            if ((GetAsyncKeyState(VK_LSHIFT) & 0x8000) && (GetAsyncKeyState('D') & 0x8000))
            {
                if (ChangeMediaState(SpotifyAppID, MediaSate::Next))
                    std::wcout << L"Successfully updated media state to: Next" << std::endl;
                else
                    std::wcout << L"Failed to update media state to: Next" << std::endl;

                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }

            if ((GetAsyncKeyState(VK_LSHIFT) & 0x8000) && (GetAsyncKeyState('F') & 0x8000))
            {
                if (ChangeMediaState(SpotifyAppID, MediaSate::Previous))
                    std::wcout << L"Successfully updated media state to: Previous" << std::endl;
                else
                    std::wcout << L"Failed to update media state to: Previous" << std::endl;

                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(13));
        }


        uninit_apartment();
    }
    catch (const winrt::hresult_error& e)
    {
        std::wcerr << L"WinRT error: " << e.message().c_str() << L" (HRESULT: " << e.code() << L")" << std::endl;
        MessageBoxW(NULL, e.message().c_str(), L"WinRT Error", MB_ICONERROR);
    }
    catch (const std::exception& e)
    {
        std::wcerr << L"Standard exception: " << e.what() << std::endl;
        MessageBoxA(NULL, e.what(), "Standard Exception", MB_ICONERROR);
    }
    catch (...)
    {
        std::wcerr << L"Unknown error occurred."<< std::endl; 
        MessageBoxW(NULL, L"An unknown error occurred", L"Error", MB_ICONERROR);
    }

    return 0;
}
