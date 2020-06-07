# Internet of Things
Members: 
- Nguyen Duy Phu Quang - 15890
- Nguyen Duc Thang - 15656
- Nguyen Anh Tai - 16128
- Bui Le Phi Long - 16619
## 1. Definition:
- The Internet of things (IoT) is a **system of interrelated computing devices, mechanical and digital machines** provided with **unique identifiers** (UIDs)( UID is any identifier which is guaranteed to be unique among all identifiers used for those objects and for a specific purpose) and the ability to **transfer data over a network** **without requiring human-to-human or human-to-computer interaction**.
- Real-World IOT Examples:
    - *IoT Sensors* :
        - IoT sensors consist of manual or digital sensors connected to circuit boards, which can be programmed to measure a range of data collected from a sensor device such as carbon monoxide, temperature, humidity, pressure, vibration, and motion.
        - IoT sensors can not only gather data at different physical environments but also send data to the connected devices.
        - IoT sensors allow seamless control of data through automation delivering actionable insights



    - *IoT Tracking and Monitoring System*:IoT asset tracking devices use GPS or radio frequency (RF) to track and monitor properties. The smart devices can be used for long-range identification and verification of assets.
    - *Smart Farming*:Farmers can use smart IoT farming applications for optimizing different activities: 
        - determining the best time to harvest plants
        - creating fertilizer profiles based on the chemistry of soil 
        - sensing soil nutrients and moisture levels.



## 2. Components of an IOT architecture: 
- **Sensors**: gather and transmit data to the gateways.
- **Actuators**: 
    - allow objects to act.
    - receive commands from the gateways.  
- **Gateways**: 
    - connect objects and the cloud.
    - can preprocess sensors data sent to the cloud and actuators commands  sent from the cloud.
- **Cloud gateways**: facilitates data compression and secure data transmission between field gateways and cloud IoT servers.
- **Streaming data processor** ensures effective transition of input data to a data lake and control applications. In this way, the occasional loss or corruption of data is prevented.
- **Data lake**: store the unrefined sensors data. 
- **Big data warehouse** :
    - store filtered,preprocessed data extracted from data lake. 
    - contains only cleaned, structured and matched data. 
    - store context information about things and sensors and the commands control applications send to things.
- **Data analytics**: 
    - use data from the big data warehouse to find trends and acquire actionable insights.
    - the manual detection of correlations and patterns in data help creating algorithms for control applications.
- **Machine learning (ML)**: utilize data in big data warehouse to learn to create more precise and more efficient models for control applications.When the applicability and efficiency of new models are testified and under the approval of data analysts, they are utilized by control applications.

- **Control applications**:  
    - send automatic commands and alerts to actuators. 
    - can be either rule-based or machine-learning based. 
- **User applications**: are a software component of an IoT system which allow end-users to monitor and control IOT objects. 
    - E.g: With a mobile or web app, users can monitor the state of their things, send commands to control applications, set the options of automatic behavior.

## 3. Applications of IOT
## 4. Risk following usage of IOT
As an IOT system scale up with more connected devices, it is prone to several factors:
-  Issues collecting and processing ever-enlarging data.
-  A small compromise in a device can lead to the collapse of the whole system.
-   Non-universal standards of devices connectivity and compatibility, making communication harder as the system scale up.
- Higher risk of being penetrated by hackers. More devices in the network means more opening for a surface attack.How can IOT systems be at risk:
    - Access to sensitive data: eg.printer, webcam ,CCTV for gathering secret intelligence that may harm the business.
    - Sabotage: deliberately make harm to your system.
        -    E.g: Wannacry 2017, lock down computers for ransom.
    - Botnets: 
        - an attackers connect multiple hacked IOT devices, e.g: video cameras , to create a network called botnet.
        - can be used for multiple things, especially DDOS ( Distributed Denial of Service):
            - Multiple devices sending streams of network requests to the servers, which overflow servers' response rate and make them crash.
            - Example: 2016 a DDOS attack brought down Netflix and Spotify.
        - methods: manufacturers leave IOT devices with default password and username. Hackers can access those devices simply by trying some popular password combinations.


