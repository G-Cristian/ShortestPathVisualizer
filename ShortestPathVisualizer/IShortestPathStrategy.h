#pragma once

class IShortestPathStrategy {
public:
	virtual ~IShortestPathStrategy() = default;

	virtual int step() = 0;
protected:
	IShortestPathStrategy() = default;
};