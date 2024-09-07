//
// Basic instrumentation profiler by Cherno
// Check readme
#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>

// Function macro
#define PROFILING 1 // Enable or disable profiling
#if PROFILING
// Whrite function execution time with custom name
#define PROFILE_SCOPE(name) ProfilingInstrumentor::Timer timer##__LINE__(name);
// Whrite function execution time with _function_ name
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCTION__)
#else
#define PROFILE_FUNCTION()
#endif

#define THREAD_SAFETY true

namespace ProfilingInstrumentor
{
    /**
     *  @brief  Struct of execution time to store in file.
     */
    struct ProfileResult
    {
        std::string Name;
        long long Start, End;
        uint32_t ThreadID;
    };

    /**
     *  @brief  Struct to distinguish session
     */
    struct Session
    {
        std::string Name;
    };

    /**
     *  @brief  Main class to write profiling results
     */
    class Instrumentor
    {
    private:
        Session *m_CurrentSession;
        std::ofstream m_OutputStream;
        int m_ProfileCount;
        bool sessionActive = false;

        std::string filepath;
        std::vector<ProfileResult> resultsBuffer;
        bool threadSafe = THREAD_SAFETY;
        mutable std::mutex m_mtx;

        /**
         *  @brief Write ProfileResult into stream
         */
        void writeResult(const ProfileResult &result)
        {
            if (m_ProfileCount++ > 0)
                m_OutputStream << ",";

            std::string name = result.Name;
            std::replace(name.begin(), name.end(), '"', '\'');

            m_OutputStream << "{";
            m_OutputStream << "\"cat\":\"function\",";
            m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
            m_OutputStream << "\"name\":\"" << name << "\",";
            m_OutputStream << "\"ph\":\"X\",";
            m_OutputStream << "\"pid\":0,";
            m_OutputStream << "\"tid\":" << result.ThreadID << ",";
            m_OutputStream << "\"ts\":" << result.Start;
            m_OutputStream << "}";

            m_OutputStream.flush();
        }

        /**
         *  @brief Write profiling header that should be inside every json file in order to be interpreted by chrome
         */
        void startWriting()
        {
            m_OutputStream.open(this->filepath);
            m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
            m_OutputStream.flush();
        }

        /**
         *  @brief Write closing footer into file
         */
        void endWriting()
        {
            m_OutputStream << "]}";
            m_OutputStream.flush();
            m_OutputStream.close();
            delete m_CurrentSession;
            m_CurrentSession = nullptr;
            m_ProfileCount = 0;
        }

    
    public:
        /*
        * Public methods
        */


        /**
         *  @brief constructor
         */
        Instrumentor()
            : m_CurrentSession(nullptr), m_ProfileCount(0), filepath("")
        {
        }

        /**
         *  @brief Initialize profiling session
         *  @param  name  The name of the session.
         *  @param  filepath  filepath to results file
         */
        void beginSession(const std::string &name, const std::string &filepath = "Output/Profiling/results.json")
        {
            m_CurrentSession = new Session{name};
            this->filepath = filepath;
            sessionActive = true;
            if (threadSafe)
            {
                //nothing
            }
            else
            {
                startWriting();
            }
        }

        /**
         *  @brief End session, write closing footer into file and close output stream
         */
        void endSession()
        {
            if (threadSafe)
            {
                //write header
                startWriting();
                //write each result
                for (unsigned i = 0; i < this->resultsBuffer.size(); i++)
                {
                    this->writeResult(this->resultsBuffer[i]);
                }                
                //write footer
                endWriting();
            }
            else
            {
                endWriting();
            }
            sessionActive = false;
        }

        /**
         *  @brief Add ProfileResult into buffer or output
         */
        void addResult(const ProfileResult &result)
        {
            if (!this->sessionActive) {
                std::cout << "WARNING::Instrumentor::addResult Called after session ended" << std::endl;
                return;
            }

            if (threadSafe)
            {
                const std::lock_guard<std::mutex> lock{m_mtx};
                this->resultsBuffer.push_back(result);
            }
            else
            {
                writeResult(result);
            }
        }

        /**
         *  @brief Get singleton
         */
        static Instrumentor &getInstance()
        {
            static Instrumentor instance;
            return instance;
        }
    };

    /**
     *  @brief Class to start, store and stop execution time
     */
    class Timer
    {
    public:
        /**
         *  @brief Constructor. Also starts the timer
         */
        Timer(const std::string name)
            : m_Name(name), m_Stopped(false)
        {
            m_StartTimepoint = std::chrono::high_resolution_clock::now();
        }

        /**
         *  @brief Destructor. Also stops the timer
         */
        ~Timer()
        {
            if (!m_Stopped)
                stop();
        }

        /**
         *  @brief Stop the timer
         */
        void stop()
        {
            auto endTimepoint = std::chrono::high_resolution_clock::now();

            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
            long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

            uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
            Instrumentor::getInstance().addResult({m_Name, start, end, threadID});

            m_Stopped = true;
        }

    private:
        const std::string m_Name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
        bool m_Stopped;
    };
}