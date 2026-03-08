#include "Shell/Audio.hpp"
#include "Utilities/Randomizer.hpp"
#include "Utilities/Log.hpp"

namespace Mines {
	Audio& Audio::getInstance() {
		static Audio instance;
		return instance;
	}

	void Audio::loadSounds(const std::string& folder) {
		std::vector<std::string> names = {
			"Click",
			"Switch",
			"Move",
			"Mark",
			"Safe",
			"Death",
			"Clear"
		};

		for (const auto& name : names) {
			sf::SoundBuffer buffer;
			
			if (buffer.loadFromFile(folder + name + ".flac")) {
				sound_buffers[name] = buffer;
			}
		}
	}

	void Audio::playSound(const std::string& name, bool random_pitch) {
		std::lock_guard<std::mutex> lock(mtx);
		auto it = sound_buffers.find(name);
		float pitch = 1.0f;

		if (it == sound_buffers.end()) {
			LOG_WARN("Sound souurce '{}' not found.", name);
			return;
		}

		if (random_pitch) {
			pitch = Randomizer::getInstance().getFloat(0.9f, 1.1f);
		}

		auto sound = std::make_unique<sf::Sound>(it->second);
		sound->setBuffer(sound_buffers[name]);
		sound->setPitch(pitch);
		sound->setVolume(sound_volume);
		sound->play();
		sounds.push_back(std::move(sound));
	}

	bool Audio::playMusic(
		const std::string& path,
		float start_offset,
		bool loop,
		float loop_start,
		float loop_end
	) {
		if (!music.openFromFile(path)) {
			return false;
		}

		if (start_offset < 0 || loop_start < 0 || loop_end < 0 || loop_end < loop_start) {
			return false;
		}

		if (loop && loop_end != 0) {
			music.setLoopPoints(
				{
					sf::seconds(loop_start),
					sf::seconds(loop_end - loop_start)
				}
			);
		}

		LOG_INFO("Music playing: {}", path);
		music.setPlayingOffset(sf::seconds(start_offset));
		music.setLooping(loop);
		music.setVolume(music_volume);
		music.play();
		return true;
	}

	void Audio::stopMusic() {
		music.stop();
	}

	void Audio::setMusicVolume(float volume) {
		std::lock_guard<std::mutex> lock(mtx);
		music_volume = volume;
		music.setVolume(music_volume);
	}

	void Audio::setSoundVolume(float volume) {
		std::lock_guard<std::mutex> lock(mtx);
		sound_volume = volume;

		for (auto& sound : sounds) {
			sound->setVolume(sound_volume);
		}
	}

	void Audio::cleanup() {
		if (clock.getElapsedTime().asSeconds() > 1.0f) {
			std::lock_guard<std::mutex> lock(mtx);

			sounds.erase(
				std::remove_if(
					sounds.begin(),
					sounds.end(),
					[](const std::unique_ptr<sf::Sound>& s) {
						return s->getStatus() == sf::Sound::Status::Stopped;
					}
				),
				sounds.end()
			);

			clock.restart();
		}
	}
} // namespace Mines