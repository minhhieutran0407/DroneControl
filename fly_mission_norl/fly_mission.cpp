/**
 * 1. Adds mission items.
 * 2. Starts mission from first mission item.
 * 3. Illustrates Pause/Resume mission item.
 * 4. Exits after the mission is accomplished.
 */

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/mission/mission.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <stdlib.h>
#include <stdio.h>

#include <functional>
#include <future>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <string>

#define ERROR_CONSOLE_TEXT "\033[31m" // Turn text on console red
#define TELEMETRY_CONSOLE_TEXT "\033[34m" // Turn text on console blue
#define NORMAL_CONSOLE_TEXT "\033[0m" // Restore normal console colour

using namespace mavsdk;
using namespace std::placeholders; // for `_1`
using namespace std::chrono; // for seconds(), milliseconds()
using namespace std::this_thread; // for sleep_for()

// Handles Action's result
inline void handle_action_err_exit(Action::Result result, const std::string& message);
// Handles Mission's result
inline void handle_mission_err_exit(Mission::Result result, const std::string& message);
// Handles Connection result
inline void handle_connection_err_exit(ConnectionResult result, const std::string& message);

static Mission::MissionItem make_mission_item(
    double latitude_deg,
    double longitude_deg,
    float relative_altitude_m,
    float speed_m_s,
    bool is_fly_through,
    float gimbal_pitch_deg,
    float gimbal_yaw_deg,
    Mission::MissionItem::CameraAction camera_action);

void usage(std::string bin_name)
{
    std::cout << NORMAL_CONSOLE_TEXT << "Usage : " << bin_name << " <connection_url>" << std::endl
              << "Connection URL format should be :" << std::endl
              << " For TCP : tcp://[server_host][:server_port]" << std::endl
              << " For UDP : udp://[bind_host][:bind_port]" << std::endl
              << " For Serial : serial:///path/to/serial/dev[:baudrate]" << std::endl
              << "For example, to connect to the simulator use URL: udp://:14540" << std::endl;
}

int main(int argc, char** argv)
{
    Mavsdk dc;

    {
        auto prom = std::make_shared<std::promise<void>>();
        auto future_result = prom->get_future();

        std::cout << "Waiting to discover system..." << std::endl;
        dc.register_on_discover([prom](uint64_t uuid) {
            std::cout << "Discovered system with UUID: " << uuid << std::endl;
            prom->set_value();
        });

        std::string connection_url;
        ConnectionResult connection_result;

        if (argc == 2) {
            connection_url = argv[1];
            connection_result = dc.add_any_connection(connection_url);
        } else {
            usage(argv[0]);
            return 1;
        }

        if (connection_result != ConnectionResult::Success) {
            std::cout << ERROR_CONSOLE_TEXT << "Connection failed: " << connection_result
                      << NORMAL_CONSOLE_TEXT << std::endl;
            return 1;
        }

        future_result.get();
    }

    dc.register_on_timeout([](uint64_t uuid) {
        std::cout << "System with UUID timed out: " << uuid << std::endl;
        std::cout << "Exiting." << std::endl;
        exit(0);
    });

    // We don't need to specifiy the UUID if it's only one system anyway.
    // If there were multiple, we could specify it with:
    // dc.system(uint64_t uuid);
    System& system = dc.system();
    auto action = std::make_shared<Action>(system);
    auto mission = std::make_shared<Mission>(system);
    auto telemetry = std::make_shared<Telemetry>(system);

    while (!telemetry->health_all_ok()) {
        std::cout << "Waiting for system to be ready" << std::endl;
        sleep_for(seconds(1));
    }

    std::cout << "System ready" << std::endl;
    std::cout << "Creating and uploading mission" << std::endl;

    std::vector<Mission::MissionItem> mission_items;

    float d_alt = 10.0f;
    float d_speed = 5.0f;

    std::ifstream ipx;
    ipx.open("/home/hieu/MAVSDK_Project_Test/data/altsp");

    while(ipx.good()){
        std::string alt;
        std::string speed;
        getline(ipx, alt, ',');
        getline(ipx, speed, '\n');

        if(alt.empty() || speed.empty()){
        break;
        }

        d_alt = std::stof(alt);
        d_speed = std::stof(speed);
        break;
    }

    ipx.close();

    std::ifstream ip;
    ip.open("/home/hieu/MAVSDK_Project_Test/data/input");

    while(ip.good()){
        std::string lon;
        std::string lat;
        getline(ip, lon, ',');
        getline(ip, lat, '\n');

        if(lat.empty() || lon.empty()){
        break;
        }

        double d_lat = std::stod(lat);
        double d_lon = std::stod(lon);

        mission_items.push_back(make_mission_item(
            d_lat,
            d_lon,
            d_alt,
            d_speed,
            false,
            20.0f,
            60.0f,
            Mission::MissionItem::CameraAction::None));
    }

    ip.close();

//    mission_items.push_back(make_mission_item(
//        47.398570327054473,
//        8.5458490218639658,
//        10.0f,
//        5.0f,
//        false,
//        20.0f,
//        60.0f,
//        Mission::MissionItem::CameraAction::None));

//    mission_items.push_back(make_mission_item(
//        47.398241338125118,
//        8.5455360114574432,
//        10.0f,
//        2.0f,
//        true,
//        0.0f,
//        -60.0f,
//        Mission::MissionItem::CameraAction::TakePhoto));

//    mission_items.push_back(make_mission_item(
//        47.398139363821485,
//        8.5453846156597137,
//        10.0f,
//        5.0f,
//        true,
//        -45.0f,
//        0.0f,
//        Mission::MissionItem::CameraAction::StartVideo));

//    mission_items.push_back(make_mission_item(
//        47.398058617228855,
//        8.5454618036746979,
//        10.0f,
//        2.0f,
//        false,
//        -90.0f,
//        30.0f,
//        Mission::MissionItem::CameraAction::StopVideo));

//    mission_items.push_back(make_mission_item(
//        47.398100366082858,
//        8.5456969141960144,
//        10.0f,
//        5.0f,
//        false,
//        -45.0f,
//        -30.0f,
//        Mission::MissionItem::CameraAction::StartPhotoInterval));

//    mission_items.push_back(make_mission_item(
//        47.398001890458097,
//        8.5455576181411743,
//        10.0f,
//        5.0f,
//        false,
//        0.0f,
//        0.0f,
//        Mission::MissionItem::CameraAction::StopPhotoInterval));

    {
        std::cout << "Uploading mission..." << std::endl;
        // We only have the upload_mission function asynchronous for now, so we wrap it using
        // std::future.
        auto prom = std::make_shared<std::promise<Mission::Result>>();
        auto future_result = prom->get_future();
        Mission::MissionPlan mission_plan{};
        mission_plan.mission_items = mission_items;
        mission->upload_mission_async(
            mission_plan, [prom](Mission::Result result) { prom->set_value(result); });

        const Mission::Result result = future_result.get();
        if (result != Mission::Result::Success) {
            std::cout << "Mission upload failed (" << result << "), exiting." << std::endl;
            return 1;
        }
        std::cout << "Mission uploaded." << std::endl;
    }

    // We want to listen to the altitude of the drone at 1 Hz.
    const Telemetry::Result set_rate_result = telemetry->set_rate_position(0.25);
    if (set_rate_result != Telemetry::Result::Success) {
        std::cout << ERROR_CONSOLE_TEXT << "Setting rate failed:" << set_rate_result
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return 1;
    }

    std::ofstream myfile;
    //myfile.open("/home/hieu/MAVSDK_Project/data/output.csv",std::ios::app);

    // Set up callback to monitor altitude while the vehicle is in flight
    telemetry->subscribe_position([&myfile](Telemetry::Position position) {
        std::cout << "Possion:" << std::endl
                  << TELEMETRY_CONSOLE_TEXT // set to blue
                  << "Altitude: " << position.relative_altitude_m << " m"
                  << std::endl
                  << "Latitude: " << position.latitude_deg << " degree"
                  << std::endl
                  << "Longitude: " << position.longitude_deg << " degree"
                  << NORMAL_CONSOLE_TEXT // set to default color again
                  << std::endl;

        myfile.open("/home/hieu/MAVSDK_Project_Test/data/output",std::ios::app);
        myfile << position.relative_altitude_m << ",";
        myfile << position.latitude_deg << ",";
        myfile << position.longitude_deg << "\n";
        myfile.close();
    });

    std::cout << "Arming..." << std::endl;
    const Action::Result arm_result = action->arm();
    handle_action_err_exit(arm_result, "Arm failed: ");
    std::cout << "Armed." << std::endl;

    // Before starting the mission, we want to be sure to subscribe to the mission progress.
    mission->subscribe_mission_progress(
        [](Mission::MissionProgress mission_progress) {
            std::cout << "Mission status update: " << mission_progress.current << " / "
                      << mission_progress.total << std::endl;
        });

    {
        std::cout << "Starting mission." << std::endl;
        auto prom = std::make_shared<std::promise<Mission::Result>>();
        auto future_result = prom->get_future();
        mission->start_mission_async([prom](Mission::Result result) {
            prom->set_value(result);
            std::cout << "Started mission." << std::endl;
        });

        const Mission::Result result = future_result.get();
        handle_mission_err_exit(result, "Mission start failed: ");
    }

    while (!mission->is_mission_finished().second) {
        sleep_for(seconds(1));
    }

    std::cout << "Landing..." << std::endl;
    const Action::Result land_result = action->land();
    if (land_result != Action::Result::Success) {
        std::cout << ERROR_CONSOLE_TEXT << "Land failed:" << land_result << NORMAL_CONSOLE_TEXT
                  << std::endl;
        return 1;
    }

    // Check if vehicle is still in air
    while (telemetry->in_air()) {
        std::cout << "Vehicle is landing..." << std::endl;
        sleep_for(seconds(1));
    }
    std::cout << "Landed!" << std::endl;

    // We need to wait a bit, otherwise the armed state might not be correct yet.
    sleep_for(seconds(2));

    while (telemetry->armed()) {
        // Wait until we're done.
        sleep_for(seconds(1));
    }
    std::cout << "Disarmed, exiting." << std::endl;
}

Mission::MissionItem make_mission_item(
    double latitude_deg,
    double longitude_deg,
    float relative_altitude_m,
    float speed_m_s,
    bool is_fly_through,
    float gimbal_pitch_deg,
    float gimbal_yaw_deg,
    Mission::MissionItem::CameraAction camera_action)
{
    Mission::MissionItem new_item{};
    new_item.latitude_deg = latitude_deg;
    new_item.longitude_deg = longitude_deg;
    new_item.relative_altitude_m = relative_altitude_m;
    new_item.speed_m_s = speed_m_s;
    new_item.is_fly_through = is_fly_through;
    new_item.gimbal_pitch_deg = gimbal_pitch_deg;
    new_item.gimbal_yaw_deg = gimbal_yaw_deg;
    new_item.camera_action = camera_action;
    return new_item;
}

inline void handle_action_err_exit(Action::Result result, const std::string& message)
{
    if (result != Action::Result::Success) {
        std::cerr << ERROR_CONSOLE_TEXT << message << result << NORMAL_CONSOLE_TEXT << std::endl;
        exit(EXIT_FAILURE);
    }
}

inline void handle_mission_err_exit(Mission::Result result, const std::string& message)
{
    if (result != Mission::Result::Success) {
        std::cerr << ERROR_CONSOLE_TEXT << message << result << NORMAL_CONSOLE_TEXT << std::endl;
        exit(EXIT_FAILURE);
    }
}

// Handles connection result
inline void handle_connection_err_exit(ConnectionResult result, const std::string& message)
{
    if (result != ConnectionResult::Success) {
        std::cerr << ERROR_CONSOLE_TEXT << message << result << NORMAL_CONSOLE_TEXT << std::endl;
        exit(EXIT_FAILURE);
    }
}
