#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 50

class Animation
{
public:
	bool loop = true;
	bool manual = false;
	float speed = 0.4f;
	SDL_Rect frames[MAX_FRAMES];
	int name;

private:
	float current_frame = 0.0f;
	int last_frame = 0;
	int loops = 0;

public:

	Animation()
	{}

	Animation(const Animation& anim) : loop(anim.loop), speed(anim.speed), last_frame(anim.last_frame)
	{
		SDL_memcpy(&frames, anim.frames, sizeof(frames));
	}

	void PushBack(const SDL_Rect& rect)
	{
		if (last_frame < MAX_FRAMES)
			frames[last_frame++] = rect;
	}

	SDL_Rect& GetCurrentFrame()
	{
		if (!manual)
		{
			current_frame += speed;

			if (current_frame >= last_frame)
			{
				current_frame = (loop) ? 0.0f : last_frame - 1;
				loops++;
			}
		}

		return frames[(int)current_frame];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		current_frame = 0.0f;
		loops = 0;
	}

	void SetFrame(int frame)
	{
		if (frame >= 0 && frame <= last_frame)
			current_frame = (float)frame;
	}
};

#endif