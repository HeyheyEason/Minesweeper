#pragma once

#include <SFML/Audio.hpp>
#include <SFML/System/Clock.hpp>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <mutex>

namespace Mines {
	class Audio {
	public:
		static Audio& getInstance();
		Audio(const Audio&) = delete;
		void operator=(const Audio&) = delete;

		void loadSounds(const std::string& folder);
		void playSound(const std::string& name, bool random_pitch = false);

		bool playMusic(
			const std::string& path,
			float start_offset = 0,
			bool loop = true,
			float loop_start = 0,
			float loop_end = 0
		);

		void stopMusic();
		void setMusicVolume(float volume);
		void setSoundVolume(float volume);
		void cleanup();

	private:
		sf::Music music;
		float music_volume = 100.0f;
		float sound_volume = 100.0f;
		std::map<std::string, sf::SoundBuffer> sound_buffers;
		std::vector<std::unique_ptr<sf::Sound>> sounds;
		sf::Clock clock;

		std::mutex mtx;
		Audio() = default;
	};
} // namespace Mines