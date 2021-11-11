#pragma once

#include	<cinttypes>
#include	<chrono>
#include	"time.h"

// using �錾
using Duration = std::chrono::duration<double>;
using MicrosecondsF = std::chrono::duration<double, std::micro>;
using SecondsF = std::chrono::duration<double>;

class Stopwatch
{
private:
	int64_t mStartTimeMicrosec = 0;
	int64_t mAccumulationMicrosec = 0;
	bool mIsStarted = false;
	bool mPausing = true;

public:

	/// <summary>
	/// �X�g�b�v�E�H�b�`���쐬���܂��B
	/// </summary>
	/// <param name="startImmediately">
	/// �����Ɍv�����J�n����ꍇ�� true
	/// </param>
	explicit Stopwatch(bool startImmediately = false)
	{
		if (startImmediately)
		{
			start();
		}
	}

	/// <summary>
	/// �X�g�b�v�E�H�b�`���쐬���܂��B
	/// </summary>
	/// <param name="startImmediately">
	/// �����Ɍv�����J�n����ꍇ�� true
	/// </param>
	explicit Stopwatch(const Duration& time, bool startImmediately = false)
	{
		set(time);

		if (startImmediately)
		{
			start();
		}
	}

	/// <summary>
	/// �X�g�b�v�E�H�b�`���J�n�E�ĊJ���܂��B
	/// </summary>
	/// <returns>
	/// �Ȃ�
	/// </returns>
	void start()
	{
		if (!mPausing)
		{
			return;
		}

		mIsStarted = true;

		mPausing = false;

		mStartTimeMicrosec = Time::GetMicrosec();
	}

	/// <summary>
	/// �X�g�b�v�E�H�b�`�̌o�ߎ��Ԃ�[��]�ŕԂ��܂��B
	/// </summary>
	/// <returns>
	/// �X�g�b�v�E�H�b�`�̌o�ߎ���[��]
	/// </returns>
	int32_t d() const { return static_cast<int32_t>(d64()); }

	int64_t d64() const { return us() / (1000LL * 1000LL * 60LL * 60LL * 24LL); }

	double dF() const { return static_cast<double>(us() / static_cast<double>(1000LL * 1000LL * 60LL * 60LL * 24LL)); }

	/// <summary>
	/// �X�g�b�v�E�H�b�`�̌o�ߎ��Ԃ�[��]�ŕԂ��܂��B
	/// </summary>
	/// <returns>
	/// �X�g�b�v�E�H�b�`�̌o�ߎ���[��]
	/// </returns>
	int32_t h() const { return static_cast<int32_t>(h64()); }

	int64_t h64() const { return us() / (1000LL * 1000LL * 60LL * 60LL); }

	double hF() const { return static_cast<double>(us() / static_cast<double>(1000LL * 1000LL * 60LL * 60LL)); }

	/// <summary>
	/// �X�g�b�v�E�H�b�`�̌o�ߎ��Ԃ�[��]�ŕԂ��܂��B
	/// </summary>
	/// <returns>
	/// �X�g�b�v�E�H�b�`�̌o�ߎ���[��]
	/// </returns>
	int32_t min() const { return static_cast<int32_t>(min64()); }

	int64_t min64() const { return us() / (1000LL * 1000LL * 60LL); }

	double minF() const { return static_cast<double>(us() / static_cast<double>(1000LL * 1000LL * 60LL)); }

	/// <summary>
	/// �X�g�b�v�E�H�b�`�̌o�ߎ��Ԃ�[�b]�ŕԂ��܂��B
	/// </summary>
	/// <returns>
	/// �X�g�b�v�E�H�b�`�̌o�ߎ���[�b]
	/// </returns>
	int32_t s() const { return static_cast<int32_t>(s64()); }

	int64_t s64() const { return us() / (1000LL * 1000LL); }

	double sF() const { return static_cast<double>(us() / static_cast<double>(1000LL * 1000LL)); }

	/// <summary>
	/// �X�g�b�v�E�H�b�`�̌o�ߎ��Ԃ�[�~���b]�ŕԂ��܂��B
	/// </summary>
	/// <returns>
	/// �X�g�b�v�E�H�b�`�̌o�ߎ���[�~���b]
	/// </returns>
	int32_t ms() const { return static_cast<int32_t>(ms64()); }

	int64_t ms64() const { return us() / (1000LL); }

	double msF() const { return static_cast<double>(us() / static_cast<double>(1000LL)); }

	/// <summary>
	/// �X�g�b�v�E�H�b�`�̌o�ߎ��Ԃ�[�}�C�N���b]�ŕԂ��܂��B
	/// </summary>
	/// <returns>
	/// �X�g�b�v�E�H�b�`�̌o�ߎ���[�}�C�N���b]
	/// </returns>
	int64_t us() const
	{
		const int64_t t = Time::GetMicrosec();

		if (!mIsStarted)
		{
			return 0;
		}

		if (mPausing)
		{
			return mAccumulationMicrosec;
		}

		return mAccumulationMicrosec + (t - mStartTimeMicrosec);
	}

	int64_t us64() const
	{
		return us();
	}

	double usF() const { return static_cast<double>(us()); }

	/// <summary>
	/// �X�g�b�v�E�H�b�`�̌o�ߎ��Ԃ�Ԃ��܂��B
	/// </summary>
	/// <returns>
	/// �X�g�b�v�E�H�b�`�̌o�ߎ���
	/// </returns>
	Duration elapsed() const { return SecondsF(sF()); }

	/// <summary>
	/// �X�g�b�v�E�H�b�`�����쒆�ł��邩�������܂��i�ꎞ��~���Ă��邱�Ƃ�����܂��j�B
	/// </summary>
	/// <remarks>
	/// �X�g�b�v�E�H�b�`���J�n����Ă���A�܂��͊J�n��ꎞ��~���ł���ꍇ true, ����ȊO�̏ꍇ�� false
	/// </remarks>
	bool isStarted() const { return mIsStarted; }

	/// <summary>
	/// �X�g�b�v�E�H�b�`���ꎞ��~���ł��邩�������܂��B
	/// </summary>
	/// <remarks>
	/// �X�g�b�v�E�H�b�`���J�n��ꎞ��~���ł���ꍇ true, ����ȊO�̏ꍇ�� false
	/// </remarks>
	bool isPaused() const { return mIsStarted && mPausing; }

	/// <summary>
	/// �X�g�b�v�E�H�b�`�����Ԃ��v�����ł��邩�������܂��B
	/// </summary>
	/// <remarks>
	/// �X�g�b�v�E�H�b�`���J�n����Ă��āA�Ȃ����ꎞ��~���łȂ��ꍇ true, ����ȊO�̏ꍇ�� false
	/// </remarks>
	bool isRunning() const { return mIsStarted && !mPausing; }

	/// <summary>
	/// �X�g�b�v�E�H�b�`���ꎞ��~���܂��B
	/// </summary>
	/// <returns>
	/// �Ȃ�
	/// </returns>
	void pause()
	{
		mAccumulationMicrosec = us();

		mPausing = true;
	}

	/// <summary>
	/// �X�g�b�v�E�H�b�`���ꎞ��~���ł���ꍇ�A�ĊJ���܂��B
	/// </summary>
	/// <returns>
	/// �Ȃ�
	/// </returns>
	void resume()
	{
		if (!isPaused())
		{
			return;
		}

		start();
	}

	/// <summary>
	/// �X�g�b�v�E�H�b�`���~���A�o�ߎ��Ԃ� 0 �Ƀ��Z�b�g���܂��B
	/// </summary>
	/// <returns>
	/// �Ȃ�
	/// </returns>
	void reset() noexcept
	{
		mAccumulationMicrosec = 0;

		mIsStarted = false;

		mPausing = true;
	}

	/// <summary>
	/// �o�ߎ��Ԃ� 0 �Ƀ��Z�b�g���āA�X�g�b�v�E�H�b�`���J�n���܂��B
	/// </summary>
	/// <returns>
	/// �Ȃ�
	/// </returns>
	void restart()
	{
		reset();

		start();
	}

	/// <summary>
	/// �X�g�b�v�E�H�b�`�̌o�ߎ��Ԃ�ύX���܂��B
	/// </summary>
	/// <param name="time">
	/// �V�����ݒ肷��o�ߎ���
	/// </param>
	/// <returns>
	/// �Ȃ�
	/// </returns>
	void set(const Duration& time)
	{
		mIsStarted = true;

		mAccumulationMicrosec = static_cast<int64_t>(time.count() * (1000LL * 1000LL));

		mStartTimeMicrosec = Time::GetMicrosec();
	}
};

bool operator <(const Stopwatch& s, const MicrosecondsF& time);

bool operator <=(const Stopwatch& s, const MicrosecondsF& time);

bool operator >(const Stopwatch& s, const MicrosecondsF& time);

bool operator >=(const Stopwatch& s, const MicrosecondsF& time);


bool operator <(const MicrosecondsF& time, const Stopwatch& s);

bool operator <=(const MicrosecondsF& time, const Stopwatch& s);

bool operator >(const MicrosecondsF& time, const Stopwatch& s);

bool operator >=(const MicrosecondsF& time, const Stopwatch& s);

