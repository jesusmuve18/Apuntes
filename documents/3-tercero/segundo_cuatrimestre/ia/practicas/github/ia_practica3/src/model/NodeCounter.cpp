#include "../../include/model/NodeCounter.h"
#include "../../include/gui/cout_colors.h"

NodeCounter *NodeCounter::instance = nullptr;

const float NodeCounter::MAX_TIME = 6000; // En segundos.
const float NodeCounter::TIME_MARGIN = 1; // En segundos.

NodeCounter::NodeCounter()
{
    accumulatedTime = 0;
    running = false;
}

NodeCounter *NodeCounter::getInstance()
{
    if (instance == nullptr)
    {
        instance = new NodeCounter();
    }
    return instance;
}

void NodeCounter::incrGenerated(int n)
{
    nodesGenerated += n;
}

void NodeCounter::incrEvaluated(int n)
{
    nodesEvaluated += n;
}

void NodeCounter::resetCounters()
{
    nodesGenerated = 0;
    nodesEvaluated = 0;
    accumulatedTime = 0;
    running = false;
}

bool NodeCounter::limitReached() const
{
    return nodeLimitReached() || timeLimitReached();
}

bool NodeCounter::nodeLimitReached() const
{
    return nodesGenerated + nodesEvaluated >= MAX_NODES; // and nodesGenerated + nodesEvaluated < MAX_NODES + NODE_MARGIN;
}

bool NodeCounter::timeLimitReached() const
{
    return accumulatedTime >= MAX_TIME; // and nodesGenerated + nodesEvaluated < MAX_TIME + TIME_MARGIN;
}

void NodeCounter::beginTimer()
{
    if (!running)
    {
        startTime = std::chrono::steady_clock::now();
        running = true;
    }
}

void NodeCounter::endTimer()
{
    if (running)
    {
        auto now = std::chrono::steady_clock::now();
        // Accumulated time in seconds (with decimal part)
        accumulatedTime += std::chrono::duration<float>(now - startTime).count();
        running = false;
    }
}

int NodeCounter::generated() const
{
    return nodesGenerated;
}

int NodeCounter::evaluated() const
{
    return nodesEvaluated;
}

float NodeCounter::time() const
{
    return accumulatedTime;
}

bool NodeCounter::limitExceeded() const{
    return nodeLimitExceeded() or timeExceeded();
}

bool NodeCounter::nodeLimitExceeded() const{
    return nodesGenerated + nodesEvaluated >= MAX_NODES + NODE_MARGIN;
}

bool NodeCounter::timeExceeded() const{
    return accumulatedTime >= MAX_TIME + TIME_MARGIN;
}

void NodeCounter::print_results(std::ostream & out) const{
    string cout_limit_ok = limitExceeded() ? COUT_RED_BOLD : limitReached() ? COUT_YELLOW_BOLD : COUT_GREEN_BOLD;
    string cout_time_ok = timeExceeded() ? COUT_RED_BOLD : timeLimitReached() ? COUT_YELLOW_BOLD : COUT_GREEN_BOLD;
    out << COUT_WHITE_BOLD << "Nodos generados: " << generated() << COUT_NOCOLOR << std::endl;
    out << COUT_WHITE_BOLD << "Nodos evaluados: " << evaluated() << COUT_NOCOLOR << std::endl;
    out << cout_limit_ok << "Total de nodos: " << generated() + evaluated() << COUT_NOCOLOR << std::endl;
    out << cout_time_ok << "Tiempo de generación+evaluación: " << time() << COUT_NOCOLOR << std::endl;

}
