package hust.hieutm.websocket.controller;

import hust.hieutm.websocket.model.Altsp;
import hust.hieutm.websocket.model.Location;
import hust.hieutm.websocket.model.Mission;
import hust.hieutm.websocket.service.ExecuteCommand;
import hust.hieutm.websocket.service.GetLocation;
import org.springframework.messaging.handler.annotation.MessageMapping;
import org.springframework.messaging.handler.annotation.SendTo;
import org.springframework.stereotype.Controller;

import java.io.IOException;

@Controller
public class LocationController {
    @MessageMapping("/send")
    @SendTo("/chat/sendMessage")
    public Location sendMessage() {

        GetLocation.getLocations();

        if (GetLocation.locations.size() > GetLocation.index){
            System.out.println(GetLocation.locations.get(GetLocation.index).getAlt());
            return GetLocation.locations.get(GetLocation.index++);
        } else{
            return new Location();
        }
    }

    @MessageMapping("/getLocation")
    @SendTo("/chat/location")
    public Location getLocation() throws IOException {
        GetLocation.currentLocation.clear();
        GetLocation.clearCurrentLocation();
        ExecuteCommand.getLocation();
        while (GetLocation.currentLocation.size() == 0){
            GetLocation.getCurrentLocation();
        }

        System.out.println(GetLocation.currentLocation.get(0).getLat() + " " + GetLocation.currentLocation.get(0).getLon());

        return GetLocation.currentLocation.get(0);
    }

    @MessageMapping("/cleardata")
    public void clearData() throws IOException {
        GetLocation.locations.clear();
        GetLocation.index = 0;
        GetLocation.clearData();
    }

    @MessageMapping("/clearmission")
    public void clearMission() throws IOException {
        GetLocation.locations.clear();
        GetLocation.index = 0;
        GetLocation.clearData();
        GetLocation.clearMission();
    }

    @MessageMapping("/takeoffandland")
    public void takeoffAndLand() throws IOException {
        ExecuteCommand.takeoffAndLand();
    }

    @MessageMapping("/mission")
    public void flyMission() throws IOException {
        ExecuteCommand.flyMission();
    }

    @MessageMapping("/missionnorl")
    public void flyMissionNorl() throws IOException {
        ExecuteCommand.flyMissionNorl();
    }

    @MessageMapping("/uploadmission")
    public void uploadMisson(Mission mission) throws Exception {
        GetLocation.uploadMission(mission.getStrMission());
    }

    @MessageMapping("/uploadaltsp")
    public void uploadAltsp(Altsp altsp) throws Exception {
        GetLocation.uploadAltsp(altsp.getStrAltsp());
    }
}
