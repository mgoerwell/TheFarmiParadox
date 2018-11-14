#include "SoundManager.h"

//defines for track filenames
#define MAIN_BGM "../Music/bgm1.wav" 
#define MENU_BGM "../Music/bgm2.wav"

//defines for SoundEffect Names
#define JUMP ""

SoundManager::SoundManager() {
    soundObject = new Sound();
    soundObject->makeBuffer(&bgmBuffer);
    soundObject->makeBuffer(&seBuffer);
    soundObject->makeSource(&bgmSource);
    soundObject->makeSource(&seSource);
}

SoundManager::~SoundManager() {
    cleanUp();
}

void SoundManager::playSong(TrackList track, float position [3]) {
    if (soundObject->isPlaying(bgmSource)) {
        soundObject->PauseAudio(bgmSource);
        soundObject->clearBuffer(bgmBuffer,bgmSource);
    }

    switch (track){
        case MainBGM:
            soundObject->bufferData(bgmBuffer, bgmSource, MAIN_BGM);
            soundObject->toggleLooping(bgmSource, true);
            soundObject->placeSource(bgmSource, position[0], position[1], position[2]);
            soundObject->PlayAudio(bgmSource);
            break;
        case MenuBGM:
            soundObject->bufferData(bgmBuffer, bgmSource, MENU_BGM);
            soundObject->toggleLooping(bgmSource, true);
            soundObject->placeSource(bgmSource, position[0], position[1], position[2]);
            soundObject->PlayAudio(bgmSource);
            break;
        default:
            break;
    }
}

void SoundManager::playSound(SoundsList soundEffect, float position[3]) {
    if (soundObject->isPlaying(seSource)) {
        soundObject->PauseAudio(seSource);
        soundObject->clearBuffer(seBuffer, seSource);
    }

    switch (soundEffect) {
    case Jump:
        soundObject->bufferData(seBuffer, bgmSource, JUMP);
        soundObject->toggleLooping(seSource, true);
        soundObject->placeSource(seSource, position[0], position[1], position[2]);
        soundObject->PlayAudio(seSource);
        break;
    default:
        break;
    }
}

void SoundManager::cleanUp() {
    //clean up step
    alDeleteSources(1, &bgmSource);
    alDeleteSources(1, &seSource);
    alDeleteBuffers(1, &bgmBuffer);
    alDeleteBuffers(1, &seBuffer);

    soundObject->~Sound();
}

void SoundManager::notify(EventName eventName, Param* param) {
    switch (eventName) {
        case PLAY_SONG: {
            TrackParams TrackInfo;
            // Safetly cast generic param pointer to a specific type
            TypeParam<TrackParams> *p = dynamic_cast<TypeParam<TrackParams> *>(param);
            if (p != nullptr) {
                // Successful type cast
                TrackInfo = p->Param;
                playSong(TrackInfo.track, TrackInfo.position);
            }
            break;
        }
        case PLAY_SOUND:{
            SoundParams SoundInfo;
            // Safetly cast generic param pointer to a specific type
            TypeParam<SoundParams> *sound = dynamic_cast<TypeParam<SoundParams> *>(param);
            if (sound != nullptr) {
                // Successful type cast
                SoundInfo = sound->Param;
                playSound(SoundInfo.sound, SoundInfo.position);
            }
            break;
        }
        default:
            break;
    }

}