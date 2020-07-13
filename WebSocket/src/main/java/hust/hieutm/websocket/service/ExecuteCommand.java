package hust.hieutm.websocket.service;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

public class ExecuteCommand {

    public static void getLocation() throws IOException {
        // creating list of commands
        List<String> commands = new ArrayList<>();
        commands.add("./takeoff_and_land"); // command
        commands.add("udp://:14540"); // command

        // creating the process
        ProcessBuilder pb = new ProcessBuilder(commands);

        // setting the directory
        pb.directory(new File("/home/hieu/MAVSDK_Project_Test/get_location/build"));

        // startinf the process
        Process process = pb.start();

        // for reading the ouput from stream
        BufferedReader stdInput = new BufferedReader(new
                InputStreamReader(process.getInputStream()));
        String s = null;
        while ((s = stdInput.readLine()) != null) {
            System.out.println(s);
        }
    }

    public static void flyMission() throws IOException {
        // creating list of commands
        List<String> commands = new ArrayList<>();
        commands.add("./fly_mission"); // command
        commands.add("udp://:14540"); // command

        // creating the process
        ProcessBuilder pb = new ProcessBuilder(commands);

        // setting the directory
        pb.directory(new File("/home/hieu/MAVSDK_Project_Test/fly_mission_np/build"));

        // startinf the process
        Process process = pb.start();

        // for reading the ouput from stream
        BufferedReader stdInput = new BufferedReader(new
                InputStreamReader(process.getInputStream()));
        String s = null;
        while ((s = stdInput.readLine()) != null) {
            System.out.println(s);
        }
    }

    public static void flyMissionNorl() throws IOException {
        // creating list of commands
        List<String> commands = new ArrayList<>();
        commands.add("./fly_mission"); // command
        commands.add("udp://:14540"); // command

        // creating the process
        ProcessBuilder pb = new ProcessBuilder(commands);

        // setting the directory
        pb.directory(new File("/home/hieu/MAVSDK_Project_Test/fly_mission_norl/build"));

        // startinf the process
        Process process = pb.start();

        // for reading the ouput from stream
        BufferedReader stdInput = new BufferedReader(new
                InputStreamReader(process.getInputStream()));
        String s = null;
        while ((s = stdInput.readLine()) != null) {
            System.out.println(s);
        }
    }

    public static void takeoffAndLand() throws IOException {
        // creating list of commands
        List<String> commands = new ArrayList<>();
        commands.add("./takeoff_and_land"); // command
        commands.add("udp://:14540"); // command

        // creating the process
        ProcessBuilder pb = new ProcessBuilder(commands);

        // setting the directory
        pb.directory(new File("/home/hieu/MAVSDK_Project_Test/takeoff_and_land/build"));

        // startinf the process
        Process process = pb.start();

        // for reading the ouput from stream
        BufferedReader stdInput = new BufferedReader(new
                InputStreamReader(process.getInputStream()));
        String s = null;
        while ((s = stdInput.readLine()) != null) {
            System.out.println(s);
        }
    }
}
