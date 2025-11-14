#ifndef IMGUI_TESTING_UTILS_H
#define IMGUI_TESTING_UTILS_H

#include <string>

typedef struct {
    bool bestAudio  = false;
    bool bestVideo  = false;
    bool worstVideo = false;
    bool worstAudio = false;
    bool best       = false;

    int selectedResolutionIndex = 0;
} VideoOptions;

typedef struct {
    std::string format = "mp3";
    std::string bitrate = "320K";
    std::string outputPath = "";
    bool embedThumbnail = false;
} AudioOptions;

typedef struct {
    std::string audioFormat = "mp3";
    std::string audioBitrate = "320K";
    std::string outputPath = "";
    std::string outputName = "\"%(playlist_index)s - %(title)s.%(ext)s\"";
    bool embedThumbnail = false;
    bool isAudioOnly = false;

    bool bestAudio = false;
    bool bestVideo = false;
    bool worstVideo = false;
    bool worstAudio = false;
    bool best = false;

    std::string playlistItems = "";
    int playlistStart = 0;
    int playlistEnd = 0;
    bool playlistReverse = false;
    bool playlistRandom = false;
    bool playlistNumbering = true;

    int selectedResolution = 0;
} PlayListOptions;

// Declare only
PlayListOptions playlistOptions;
VideoOptions videoOptions;
AudioOptions audioOptions;

const char* resolutions[] = { "2160p (4K)", "1440p", "1080p", "720p", "480p", "360p", "240p" };
const char* audioFormats[] = { "mp3", "wav", "aac", "flac", "opus" };
const char* bitrates[] = { "64K", "128K", "192K", "256K", "320K" };

std::string buildFormatVideo(const VideoOptions& opt)
{
    if (opt.best)
        return "-f best";

    if (opt.worstVideo || opt.worstAudio)
    {
        const std::string f = "-f ";
        if (opt.worstVideo && opt.worstAudio) return f + "worst";
        if (opt.worstVideo)  return f + "wv*";
        if (opt.worstAudio)  return f + "wa*";
    }

    if (opt.bestVideo || opt.bestAudio)
    {
        const std::string f = "-f ";
        if (opt.bestVideo && opt.bestAudio) return f + "bestvideo+bestaudio";

        if (opt.bestVideo) return f + "bv*";
        if (opt.bestAudio) return f + "ba*";
    }

    int h = 0;
    switch (opt.selectedResolutionIndex)
    {
        case 0: h = 2160; break; // 4K
        case 1: h = 1440; break;
        case 2: h = 1080; break;
        case 3: h = 720;  break;
        case 4: h = 480;  break;
        case 5: h = 360;  break;
        case 6: h = 240;  break;
        default: h = 1080;
    }

    if (h > 0)
    {
        return "-f \"bv[height=" + std::to_string(h) + "]+ba/best\"";
    }

    return "-f best";
}

std::string buildPlaylistFormat(const PlayListOptions& opt)
{
    if (opt.isAudioOnly)
    {
        std::string fmt = "-f ba";
        fmt += " --extract-audio --audio-format " + opt.audioFormat;
        fmt += " --audio-quality " + opt.audioBitrate;
        return fmt;
    }

    if (opt.best)
        return "-f bestvideo+bestaudio/best";

    if (opt.worstVideo || opt.worstAudio)
    {
        if (opt.worstVideo && opt.worstAudio)
            return "-f worst";

        if (opt.worstVideo)
            return "-f wv*";

        if (opt.worstAudio)
            return "-f wa*";
    }


    if (opt.bestVideo || opt.bestAudio)
    {
        if (opt.bestVideo && opt.bestAudio)
            return "-f bestvideo+bestaudio";

        if (opt.bestVideo)
            return "-f bv*";

        if (opt.bestAudio)
            return "-f ba*";
    }

    int height = 0;
    switch (opt.selectedResolution)
    {
        case 0: height = 2160; break;
        case 1: height = 1440; break;
        case 2: height = 1080; break;
        case 3: height = 720;  break;
        case 4: height = 480;  break;
        case 5: height = 360;  break;
        case 6: height = 240;  break;
        default: height = 1080; break;
    }

    return "-f \"bv[height=" + std::to_string(height) + "]+ba/best\"";
}


std::string buildPlaylistCommand( const PlayListOptions& opt)
{
    std::string cmd = " ";
    cmd += buildPlaylistFormat(opt);
    cmd += " ";

    if (!opt.outputName.empty())
        cmd += "--output \"" + opt.outputName + "\" ";

    if (opt.embedThumbnail)
        cmd += "--embed-thumbnail ";

    if (!opt.outputPath.empty())
        cmd += "-P " + opt.outputPath + " ";

    if (!opt.playlistItems.empty())
        cmd += "--playlist-items \"" + opt.playlistItems + "\" ";

    if (opt.playlistStart > 0)
        cmd += "--playlist-start " + std::to_string(opt.playlistStart) + " ";

    if (opt.playlistEnd > 0)
        cmd += "--playlist-end " + std::to_string(opt.playlistEnd) + " ";

    if (opt.playlistReverse)
        cmd += "--playlist-reverse ";

    if (opt.playlistRandom)
        cmd += "--playlist-random ";

    if (!opt.playlistNumbering)
        cmd += "--no-playlist-numbering ";

    return cmd;
}

std::string buildFormatAudio(const AudioOptions& opt)
{
    std::string res = "-x ";
    res += "--audio-format " + opt.format + " --audio-quality " + opt.bitrate + " ";

    if (opt.embedThumbnail)
        res += " --embed-thumbnail ";

    return res;
}
#endif
