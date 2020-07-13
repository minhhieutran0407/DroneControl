package hust.hieutm.websocket.service;

import hust.hieutm.websocket.model.Location;

import java.io.*;
import java.util.ArrayList;
import java.util.List;

public class GetLocation {

//    public static void main(String... args) {
//        getLocations();
//        // let's print all the person read from CSV file
//        System.out.println(locations.size());
//        for (Location loca : locations) {
//            System.out.println(loca.getAlt() + " " + loca.getLat() + " " + " " + loca.getLon());
//        }
//    }

    public static List<Location> locations = new ArrayList<>();
    public static int index = 0;

    private static Location createLocation(String[] metadata) {
        double alt = Double.parseDouble(metadata[0]);
        double lat = Double.parseDouble(metadata[1]);
        double lon = Double.parseDouble(metadata[2]);
        // create and return Location of this metadata
        return new Location(alt, lat, lon);
    }

    public static List<Location> currentLocation = new ArrayList<>();

    public static void getCurrentLocation() {

        String textFile = "/home/hieu/MAVSDK_Project_Test/data/location";
        BufferedReader br = null;
        String line = "";
        String textSplitBy = ",";

        try {
            br = new BufferedReader(new FileReader(textFile));
            while ((line = br.readLine()) != null) {
                // use comma as separator
                String[] attributes = line.split(textSplitBy);
                if(attributes.length == 3){
                    if(attributes[0] != "" && attributes[1] != "" && attributes[2] != ""){
                        Location location = createLocation(attributes);
                        currentLocation.add(location);
                    }
                }
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (br != null) {
                try {
                    br.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public static void clearCurrentLocation() throws IOException {
        FileWriter fwI = new FileWriter("/home/hieu/MAVSDK_Project_Test/data/location", false);
        PrintWriter pwI = new PrintWriter(fwI, false);
        pwI.flush();
        pwI.close();
        fwI.close();
        System.out.println("Clear current location");
    }

    public static void getLocations() {

        String textFile = "/home/hieu/MAVSDK_Project_Test/data/output";
        BufferedReader br = null;
        String line = "";
        String textSplitBy = ",";

        locations.clear();

        try {
            br = new BufferedReader(new FileReader(textFile));
            while ((line = br.readLine()) != null) {
                // use comma as separator
                String[] attributes = line.split(textSplitBy);
                if(attributes.length == 3){
                    if(attributes[0] != "" && attributes[1] != "" && attributes[2] != ""){
                        Location location = createLocation(attributes);
                        locations.add(location);
                    }
                }
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (br != null) {
                try {
                    br.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public static void clearData() throws IOException {
        FileWriter fwO = new FileWriter("/home/hieu/MAVSDK_Project_Test/data/output", false);
        PrintWriter pwO = new PrintWriter(fwO, false);
        pwO.flush();
        pwO.close();
        fwO.close();
        System.out.println("Clear data output");
    }

    public static void clearMission() throws IOException {
        FileWriter fwI = new FileWriter("/home/hieu/MAVSDK_Project_Test/data/input", false);
        PrintWriter pwI = new PrintWriter(fwI, false);
        pwI.flush();
        pwI.close();
        fwI.close();
        System.out.println("Clear mission");
    }

    public static void uploadMission(String mission) throws Exception {
        clearMission();
        File file = new File("/home/hieu/MAVSDK_Project_Test/data/input");
        FileWriter fr = new FileWriter(file, true);
        fr.write(mission);
        fr.close();
        System.out.println("Update mission");
    }

    public static void clearAltsp() throws IOException {
        FileWriter fwI = new FileWriter("/home/hieu/MAVSDK_Project_Test/data/altsp", false);
        PrintWriter pwI = new PrintWriter(fwI, false);
        pwI.flush();
        pwI.close();
        fwI.close();
        System.out.println("Clear altitude and speed");
    }

    public static void uploadAltsp(String data) throws Exception {
        clearAltsp();
        File file = new File("/home/hieu/MAVSDK_Project_Test/data/altsp");
        FileWriter fr = new FileWriter(file, true);
        fr.write(data);
        fr.close();
        System.out.println("Update Altitude & Speed");
    }
}