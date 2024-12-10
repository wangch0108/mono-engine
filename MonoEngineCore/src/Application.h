#pragma once

class Application
{
public:
	Application() : _isRunning(true), _frameCount(0) {}
	~Application() { Deinit(); }
	void Init();
	void Deinit();

private:
	void Update();
	bool _isRunning;
	uint64_t _frameCount;
};

Application& GetApplication();