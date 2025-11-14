#include "gui/gui.h"
#include "utils/utils.h"

#include "ImGuiFileDialog.h"
#include "style.h"


namespace toggles {
    bool playlistSettings = false;
    bool videoSettings    = false;
    bool audioSettings    = false;
}

int main()
{
    gui::CreateWindow("Test window", gui::WIDTH, gui::HEIGHT);
    gui::CreateImGui();
    applyTheme();

    std::string command = "yt-dlp ";
    std::string output_path;
    char url[4012] = "url";

    while (gui::isRunning)
    {
        gui::BeginRender();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(gui::WIDTH, gui::HEIGHT));
        ImGui::Begin("yt-dlp downloader", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

        // ---- Begin sidebar----
        ImGui::BeginChild("Sidebar", ImVec2(gui::WIDTH / 4, gui::HEIGHT - 50), true);

        ImGui::PushItemWidth(-80);
        ImGui::InputText("##url", url, 4012, ImGuiInputTextFlags_None);
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::SetWindowFontScale(0.85f);

        if (ImGui::Button("Clipboard", ImVec2(80, 0))) {
            std::string clipboard = ImGui::GetClipboardText();
            strncpy(url, clipboard.c_str(), 4011);
            url[4011] = '\0';
        }
        ImGui::SetWindowFontScale(1.0f);

        ImGui::Spacing();

        if (ImGui::Button("Download Video", ImVec2(170, 0))) {
            command = "yt-dlp ";
            command += url;
            command += " --output \"%(uploader)s - %(title)s.%(ext)s\" ";

            if (!output_path.empty())
                command += " -P " + output_path + " ";

            command += " " + buildFormatVideo(videoOptions);

            std::cout << command << std::endl;
            std::thread commandThread([&command]() {
                std::system(command.c_str());
            });

            commandThread.detach();
        }

        if (ImGui::Button("Download Audio", ImVec2(170, 0))) {
            command = "yt-dlp ";
            command += url;
            command += " --output \"%(uploader)s - %(title)s.%(ext)s\" ";

            if (!audioOptions.outputPath.empty())
                command += " -P " + audioOptions.outputPath + "";

            command += " " + buildFormatAudio(audioOptions);

            std::thread commandThread([&command]() {
                std::system(command.c_str());
            });

            commandThread.detach();
        }

        if (ImGui::Button("Download Playlist", ImVec2(170, 0))) {
            command = "yt-dlp ";
            command += url;
            command += " " + buildPlaylistCommand(playlistOptions);

            std::thread commandThread([&command]() {
                std::system(command.c_str());
            });

            commandThread.detach();

            command = "yt-dlp ";
        }

        ImGui::EndChild();
        // ---- End sidebar ----

        ImGui::SameLine();

        // ---- Begin Main Panel ---
        float mainPanelWidth = gui::WIDTH - (gui::WIDTH / 4) - 30;
        ImGui::BeginChild("MainPanel", ImVec2(mainPanelWidth, gui::HEIGHT - 50), true);

        if (ImGui::BeginTabBar("MainTabs"))
        {
            if (ImGui::BeginTabItem("Video settings")) {
                ImGui::Text("Video Settings");
                ImGui::Separator();

                ImGui::Spacing();

                ImGui::Text("Video / Audio Quality:");
                ImGui::Checkbox("Best Video", &videoOptions.bestVideo);
                ImGui::Checkbox("Worst Video", &videoOptions.worstVideo);
                ImGui::Checkbox("Best Audio", &videoOptions.bestAudio);
                ImGui::Checkbox("Worst Audio", &videoOptions.worstAudio);
                ImGui::Checkbox("Best Overall", &videoOptions.best);

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                ImGui::Text("Select Resolution:");

                static int resoultionIndex = 0;
                ImGui::Combo("##res_fmt", &resoultionIndex, resolutions, IM_ARRAYSIZE(resolutions));
                videoOptions.selectedResolutionIndex = resoultionIndex;

                ImGui::Separator();
                ImGui::Text("Output path: ");
                ImGui::SameLine();
                ImGui::Text("%s", output_path.c_str());
                ImGui::SameLine();
                if (ImGui::Button("Select"))
                {
                    ImGui::SetNextWindowSize(ImVec2(gui::WIDTH / 2, gui::HEIGHT / 2), ImGuiCond_Always);
                    IGFD::FileDialogConfig config;
                    config.path = ".";
                    config.flags = ImGuiFileDialogFlags_OptionalFileName | ImGuiFileDialogFlags_ReadOnlyFileNameField | ImGuiFileDialogFlags_Modal;
                    ImGuiFileDialog::Instance()->OpenDialog("OutputPath", "Select Output Folder", "", config);
                }

                if (ImGuiFileDialog::Instance()->Display("OutputPath"))
                {
                    if (ImGuiFileDialog::Instance()->IsOk())
                        output_path = ImGuiFileDialog::Instance()->GetCurrentPath();
                    ImGuiFileDialog::Instance()->Close();
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Audio settings")) {
                ImGui::Text("Audio Settings");
                ImGui::Separator();

                ImGui::Text("Format:");
                static int audioFmtIndex = 0;

                ImGui::Combo("##audio_fmt", &audioFmtIndex, audioFormats, IM_ARRAYSIZE(audioFormats));
                audioOptions.format = audioFormats[audioFmtIndex];

                ImGui::Separator();

                ImGui::Text("Bitrate:");
                static int bitrateIdx = 4;

                ImGui::Combo("##audio_bitrate", &bitrateIdx, bitrates, IM_ARRAYSIZE(bitrates));
                audioOptions.bitrate = bitrates[bitrateIdx];

                ImGui::Separator();

                ImGui::Checkbox("Embed Thumbnail", &audioOptions.embedThumbnail);

                ImGui::Separator();

                ImGui::Text("Output path: %s", audioOptions.outputPath.c_str());
                ImGui::SameLine();

                if (ImGui::Button("Select"))
                {
                    ImGui::SetNextWindowSize(ImVec2(gui::WIDTH / 2, gui::HEIGHT / 2), ImGuiCond_Always);
                    IGFD::FileDialogConfig config;
                    config.path = ".";
                    config.flags = ImGuiFileDialogFlags_OptionalFileName | ImGuiFileDialogFlags_ReadOnlyFileNameField | ImGuiFileDialogFlags_Modal;

                    ImGuiFileDialog::Instance()->OpenDialog("AudioOutput", "Select Output Folder", "", config);
                }

                if (ImGuiFileDialog::Instance()->Display("AudioOutput"))
                {
                    if (ImGuiFileDialog::Instance()->IsOk())
                        audioOptions.outputPath = ImGuiFileDialog::Instance()->GetCurrentPath();

                    ImGuiFileDialog::Instance()->Close();
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Playlist settings")) {
                ImGui::Text("Playlist Settings");
                ImGui::Separator();
                ImGui::Spacing();

                ImGui::Text("Items (e.g. \"1-10\", \"1,2,5\", \"all\")");
                ImGui::InputText("##playlist_items",
                                 playlistOptions.playlistItems.data(),
                                 playlistOptions.playlistItems.capacity() + 1);

                ImGui::InputInt("Start index", &playlistOptions.playlistStart);
                ImGui::InputInt("End index", &playlistOptions.playlistEnd);

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                ImGui::Checkbox("Reverse", &playlistOptions.playlistReverse);
                ImGui::Checkbox("Random Order", &playlistOptions.playlistRandom);
                ImGui::Checkbox("Numbering", &playlistOptions.playlistNumbering);

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                ImGui::Checkbox("Audio Only (no video)", &playlistOptions.isAudioOnly);

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                int audioFmtIndex = 0;
                ImGui::Text("Audio Format:");

                ImGui::Combo("##audio_fmt", &audioFmtIndex, audioFormats, IM_ARRAYSIZE(audioFormats));
                audioOptions.format = audioFormats[audioFmtIndex];

                ImGui::Text("Bitrate:");
                static int bitrateIdx = 4;

                ImGui::Combo("##audio_bitrate", &bitrateIdx, bitrates, IM_ARRAYSIZE(bitrates));
                audioOptions.bitrate = bitrates[bitrateIdx];

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                ImGui::Text("Resolution:");

                static int resoultionIndex = 0;
                ImGui::Combo("##playlist_fmt", &resoultionIndex, resolutions, IM_ARRAYSIZE(resolutions));
                playlistOptions.selectedResolution = resoultionIndex;


                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                ImGui::Text("Quality:");
                ImGui::Checkbox("Best Audio",  &playlistOptions.bestAudio);
                ImGui::Checkbox("Best Video",  &playlistOptions.bestVideo);
                ImGui::Checkbox("Worst Audio", &playlistOptions.worstAudio);
                ImGui::Checkbox("Worst Video", &playlistOptions.worstVideo);
                ImGui::Checkbox("Best Overall", &playlistOptions.best);

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                ImGui::Text("Output name pattern:");
                ImGui::InputText("##playlist_output_name",
                                 playlistOptions.outputName.data(),
                                 playlistOptions.outputName.capacity() + 1);

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                ImGui::Checkbox("Embed Thumbnail", &playlistOptions.embedThumbnail);

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                ImGui::Text("Output path: %s", playlistOptions.outputPath.c_str());
                ImGui::SameLine();

                if (ImGui::Button("Select"))
                {
                    ImGui::SetNextWindowSize(ImVec2(gui::WIDTH / 2, gui::HEIGHT / 2), ImGuiCond_Always);
                    IGFD::FileDialogConfig config;
                    config.path  = ".";
                    config.flags = ImGuiFileDialogFlags_OptionalFileName | ImGuiFileDialogFlags_ReadOnlyFileNameField | ImGuiFileDialogFlags_Modal;

                    ImGuiFileDialog::Instance()->OpenDialog("PlaylistOutput", "Select Output Folder", "", config);
                }

                if (ImGuiFileDialog::Instance()->Display("PlaylistOutput"))
                {
                    if (ImGuiFileDialog::Instance()->IsOk())
                        playlistOptions.outputPath = ImGuiFileDialog::Instance()->GetCurrentPath();

                    ImGuiFileDialog::Instance()->Close();
                }

                ImGui::EndTabItem();
            }

        }
        ImGui::EndTabBar();

        ImGui::EndChild();
        // ---- End main content panel ----

        ImGui::End(); // end main window
        gui::EndRender();
    }

    gui::DestroyImGui();
    gui::DestroyWindow();
    return 0;
}
