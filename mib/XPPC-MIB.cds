%--PPC-MIB { iso(1) org(3) dod(6) internet(1) private(4)
-- enterprises(1) ppc(935) }
-- Title: PPC TOP LEVEL MIB
-- Date: January 07, 1994    Version 1.00
--
-- Date: January 17, 1994    Version 1.01
--
-- Date: July 14, 1994    Version 1.02
--
-- Date: May 16, 2003    Version 2.00
                 %"WARNING: The UPS has enabled Boost."           �--#TYPE "PPC UPS: SmartBoost"
--#SUMMARY "The UPS has enabled Boost."
--#ARGUMENTS { }
--#SEVERITY MINOR
--#TIMEINDEX 1
--#HELP ""
--#HELPTAG 0
--#STATE OPERATIONAL
 8"WARNING: The UPS has switched to battery backup power."           �--#TYPE "PPC UPS: On battery"
--#SUMMARY "The UPS has switched to battery backup power."
--#ARGUMENTS { }
--#SEVERITY MAJOR
--#TIMEINDEX 1
--#HELP ""
--#HELPTAG 0
--#STATE OPERATIONAL
 ;"SEVERE: The UPS failed its internal diagnostic self-test."           �--#TYPE "PPC UPS: Failed self-test"
--#SUMMARY "The UPS has failed its internal diagnostic self-test."
--#ARGUMENTS { }
--#SEVERITY CRITICAL
--#TIMEINDEX 1
--#HELP ""
--#HELPTAG 0
--#STATE DEGRADED
 !"WARNING: UPS temperature Normal"             R"INFORMATION: The UPS has returned from a low battery
                condition."           �--#TYPE "PPC UPS: Battery normal"
--#SUMMARY "The UPS has returned from a low battery condition."
--#ARGUMENTS { }
--#SEVERITY INFORMATIONAL
--#TIMEINDEX 1
--#HELP ""
--#HELPTAG 0
--#STATE OPERATIONAL
 "WARNING: Smoke is abnormal."             ?"WARNING: The environment temperature exceed the normal value."              "WARNING: UPS Schedule Shutdown"             "WARNING: Security7 Alarm."              "WARNING: Security is abnormal."             "WARNING: Security6 Alarm."             "WARNING: Security5 Alarm."             "WARNING: Security4 Alarm."             "WARNING: Security3 Alarm."             "WARNING: Security2 Alarm."             "The UPS Low Battery Shutdown"             "WARNING: Security1 Alarm."             "WARNING: Bypass AC Normal"             #"WARNING: Rectifier Rotation Error"             "INFORMATION: Water is normal."             d"SEVERE: The UPS has sensed a load greater than 100 percent
                of its rated capacity."           �--#TYPE "PPC UPS: UPS overload"
--#SUMMARY "The UPS has sensed a load greater than 100 percent of its rated capacity."
--#ARGUMENTS { }
--#SEVERITY CRITICAL
--#TIMEINDEX 1
--#HELP ""
--#HELPTAG 0
--#STATE DEGRADED
 a"WARNING: The UPS is entering 'sleep' mode.  Power
                to the load will be cut off."           �--#TYPE "PPC UPS: Going to sleep"
--#SUMMARY "The UPS is going into sleep mode."
--#ARGUMENTS { }
--#SEVERITY MAJOR
--#TIMEINDEX 1
--#HELP ""
--#HELPTAG 0
--#STATE NONOPERATIONAL
 /"INFORMATION: Utility power has been restored."           �--#TYPE "PPC UPS: Utility restored"
--#SUMMARY "Normal power has been restored to the UPS."
--#ARGUMENTS { }
--#SEVERITY INFORMATIONAL
--#TIMEINDEX 1
--#HELP ""
--#HELPTAG 0
--#STATE OPERATIONAL
 "WARNING: Bypass AC Abnormal"             5"INFORMATION: The environment temperature is normal."             "The UPS High DC Shutdown"             !"The UPS return from Bypass Mode"             P"INFORMATION: Communication with the UPS has been
                established."           �--#TYPE "PPC UPS: Communication established"
--#SUMMARY "Communication with the UPS has been established."
--#ARGUMENTS { }
--#SEVERITY INFORMATIONAL
--#TIMEINDEX 1
--#HELP ""
--#HELPTAG 0
--#STATE OPERATIONAL
 5"INFORMATION: The UPS passed its internal self-test."           �--#TYPE "PPC UPS: Passed self-test"
--#SUMMARY "The UPS has passed its internal self-test."
--#ARGUMENTS { }
--#SEVERITY INFORMATIONAL
--#TIMEINDEX 1
--#HELP ""
--#HELPTAG 0
--#STATE OPERATIONAL
 <"WARNING: The environment humidity exceed the normal value."             "WARNING: Gas Alarm"             "The UPS Capacity Underrun"             '"The UPS Inverter Output Fail Shutdown"             "The UPS Overload Shutdown"             "WARNING: UPS Test"             ""WARNING: UPS Temperature Overrun"             ;"WARNING: The environment humidity below the normal value."             y"SEVERE: The UPS just started a runtime calibration
                discharge.  The UPS batteries are being discharged."           �--#TYPE "PPC UPS: Deep discharge"
--#SUMMARY "The UPS has just started a runtime calibration discharge."
--#ARGUMENTS { }
--#SEVERITY CRITICAL
--#TIMEINDEX 1
--#HELP ""
--#HELPTAG 0
--#STATE DEGRADED
 "The UPS Capacity Normal"             T"WARNING: The UPS has been turned 'off' by the
                management station."           �--#TYPE "PPC UPS: Switching off"
--#SUMMARY "The UPS is being switched off by a management station."
--#ARGUMENTS { }
--#SEVERITY MAJOR
--#TIMEINDEX 1
--#HELP ""
--#HELPTAG 0
--#STATE NONOPERATIONAL
 2"INFORMATION: The environment humidity is normal."             h"INFORMATION: The UPS woke up from 'sleep' mode.  Power
                to the load has been restored."           �--#TYPE "PPC UPS: Wake up"
--#SUMMARY "The UPS has woke up from sleep mode."
--#ARGUMENTS { }
--#SEVERITY INFORMATIONAL
--#TIMEINDEX 1
--#HELP ""
--#HELPTAG 0
--#STATE OPERATIONAL
 "WARNING: UPS Load Normal"             &"The UPS Static Switch in Bypass Mode"              "WARNING: Bypass Frequency Fail"             >"WARNING: The environment temperature below the normal value."             ~"WARNING: The UPS has started its reboot sequence.  After
                the specified delay the UPS will perform a reboot."           �--#TYPE "PPC UPS: Starting reboot"
--#SUMMARY "The UPS has started its reboot sequence."
--#ARGUMENTS { }
--#SEVERITY MAJOR
--#TIMEINDEX 1
--#HELP ""
--#HELPTAG 0
--#STATE NONOPERATIONAL
 -"WARNING: The UPS is entering 'bypass' mode."             +"The UPS Manual Bypass Breaker on Shutdown"              "The UPS Short Circuit Shutdown"             #"The UPS Over Temperature Shutdown"             "WARNING: Water is abnormal."             �"SEVERE: The UPS batteries are low and will soon be exhausted.
                If utility power is not restored the UPS will put itself
                to 'sleep' and immediately cut power to the load."           �--#TYPE "PPC UPS: Low battery"
--#SUMMARY "The UPS batteries are low and will soon be exhausted."
--#ARGUMENTS { }
--#SEVERITY CRITICAL
--#TIMEINDEX 1
--#HELP ""
--#HELPTAG 0
--#STATE DEGRADED
 ("The UPS Static Switch in Inverter Mode"             "The UPS Emergency Stop"             w"SEVERE: Communication to the UPS has been lost.  Steps
                to reestablish communication are in progress."           �--#TYPE "PPC UPS: Communication failure"
--#SUMMARY "Communication to the UPS has been lost."
--#ARGUMENTS { }
--#SEVERITY CRITICAL
--#TIMEINDEX 1
--#HELP ""
--#HELPTAG 0
--#STATE DEGRADED
                                                     6"The UPS model name (e.g. 'Intelligent 8000E 900VA')."                       i"An 8 byte ID string identifying the UPS.  This object
                can be set by the administrator."                           *"The firmware revision of the UPS system."                       <"The date when the UPS was manufactured in mm/dd/yy format."                      "An 8-character string identifying the serial number of
                the UPS internal microprocessor.  This number is set at
                the factory.  NOTE: This number does NOT correspond to
                the serial number on the rear of the UPS."                       4"The firmware revision of the UPS SNMP Proxy agent."                              |"The status of the UPS batteries.  A batteryLow(3)
                value indicates the UPS will be unable to sustain the
                current load, and its services will be lost if power is
                not restored.  The amount of run time in reserve at the
                time of low battery can be configured by the
                upsSmartConfigLowBatteryRunTime."                       ["The elapsed time in seconds since the UPS has switched
                to battery power."                      "The date when the UPS system's batteries were last replaced
                in mm/dd/yy format.   For UPS models, this value is originally
                set in the factory.  When the UPS batteries are replaced,
                this value should be reset by the administrator."                           X"The remaining battery capacity expressed in
                percent of full capacity."                       4"The current battery voltage expressed in 1/10 VDC."                       `"The current internal UPS temperature expressed in tenths of
                a Celsius degree."                       \"The UPS battery run time remaining before battery
                exhaustion, in seconds."                       5"Indicates whether the UPS batteries need replacing."                       z"The fully charged battery voltage of the battery system
                used in the UPS, expressed in tenths of a volt."                       W"The current battery current expressed in percent of
                maximum current."                               "The current AC input phase."                           /"The current utility line voltage in 1/10 VAC."                       b"The maximum utility line voltage in 1/10 VAC over the
                previous 1 minute period."                       b"The minimum utility line voltage in 1/10 VAC over the
                previous 1 minute period."                       ;"The current input frequency to the UPS system in 1/10 Hz."                      /"The reason for the occurrence of the last transfer
                to UPS battery power.  The variable is set to
                noTransfer(1) if there is no transfer yet.   The variable
                is set to highLineVoltage(2) if the transfer to battery is
                caused by an over voltage greater than the high transfer
                voltage. The variable is set to brownout(3) if the duration
                of the outage is greater than five seconds and the line
                voltage is between 40 percent of the rated output voltage and
                the low transfer voltage. The variable is set to blackout(4)
                if the duration of the outage is greater than five seconds and
                the line voltage is between 40 percent of the rated output voltage
                and ground.  The variable is set to smallMomentarySag(5)
                if the duration of the outage is less than five seconds and
                the line voltage is between 40 percent of the rated output voltage
                and the low transfer voltage.  The variable is set to
                deepMomentarySag(6) if the duration of the outage is less
                than five seconds and the line voltage is between 40 percent of the
                rated output voltage and ground."                               �"The current state of the UPS.  If the UPS is unable
                to determine the state of the UPS this variable is set
                to unknown(1)."                       "The current output phase."                           3"The output voltage of the UPS system in 1/10 VAC."                       <"The current output frequency of the UPS system in 1/10 Hz."                       O"The current UPS load expressed in percent
                of rated capacity."                               6"The number of devices that are plugged into the UPS."                       �"A list of devices that are plugged into the UPS.  The
                number of entries is given by the value of
                upsBaseConfigNumDevices."                       $"The devices plugged in to the UPS."                       7"The index of the device that is plugged into the UPS."                       :"The name/description of the device plugged into the UPS."                       9"The volt-amp rating of the device plugged into the UPS."                       7"An entry is added if yes, the entry is deleted if no."                          �"The nominal output voltage from the UPS in 1/10 VAC.
                Possible values are 1000, 1200, 2080, 2200, 2250, 2300
                and 2400.
                                
                NOTE: Only units that are 2200, 2250, 2300 and 2400 can
                be changed.  Allowable values are 2200, 2250, 2300 and
                2400.
                                
                For these adjustable units, if a value other than
                a supported value is provided in a set request, the
                UPS interprets it as the next lower acceptable value.
                If the provided value is lower than the lowest acceptable
                value, the lowest acceptable value is used."                      Q"The maximum line voltage in 1/10 VAC allowed before the
                UPS system transfers to battery backup.
                                
                Allowed value depend on the UPS used:
                120 Volt units allow settings of 1290, 1320, 1350, 1380.
                100 Volt units allow settings of 1080, 1100, 1120, 1140.
                230 Volt units allow settings of 2530, 2640, 2710, 2800.
                208 Volt units allow settings of 2240, 2290, 2340, 2390.
                                
                If a value other than a supported value is provided in a
                set request, the UPS interprets it as the next lower
                acceptable value.  If the provided value is lower than
                the lowest acceptable value, the lowest acceptable
                value is used."                      V"The minimum line voltage in 1/10 VAC allowed before the
                UPS system transfers to battery backup.
                                
                Allowable values depend on the UPS used:
                120 Volt units allow settings of  970, 1000, 1030, 1060.
                100 Volt units allow settings of 810, 830, 850, 870.
                230 Volt units allow settings of 1880, 1960, 2040, 2080.
                208 Volt units allow settings of 1680, 1720, 1770, 1820.
                                
                If a value other than a supported value is provided in a
                set request, the UPS interprets it as a the next higher
                acceptable value.  If the provided value is higher than
                the highest acceptable value, the highest acceptable
                value is used."                       X"A flag indicating how the UPS should handle audible line fail
                alarms."                      m"The time in seconds after initial line failure at which the
                UPS begins emitting audible alarms (beeping).  This timer is
                observerd only if the value of extControlAlarm is timed(2).
                Allowed values are 0 or 30 seconds.
                                
                If a value other than a supported value is provided in a
                set request, the UPS interprets it as a the next lower
                acceptable value.  If the provided value is lower than
                the lowest acceptable value, the lowest acceptable
                value is used."                      �"The minimum battery capacity as a percent of full capacity
                required before the UPS will return from a low battery
                shutdown condition.  In other words, the UPS will not
                re-energize the output until the battery capacity is equal
                to this value. Allowed values are 0, 10, 25, or 90  percent.
                                
                If a value other than a supported value is provided in a
                set request, the UPS interprets it as a the next higher
                acceptable value.  If the provided value is higher than
                the highest acceptable value, the highest acceptable
                value is used."                       V"The sensitivity of the UPS to utility line abnormalities
                or noises."                      p"The desired run time in seconds once the low battery
                condition is reached.  Allowed values are 120, 300, 420,
                or 600 seconds.  During this time the UPS will produce a
                constant warning tone which can not be disabled.
                                
                If a value other than a supported value is provided in a
                set request, the UPS interprets it as a the next higher
                acceptable value.  If the provided value is higher than
                the highest acceptable value, the highest acceptable
                value is used."                      �"The delay in seconds after utility line power returns
                before the UPS will turn on.  This value is also used
                when the UPS comes out of a reboot and before the UPS
                wakes up from 'sleep' mode.  Allowed values are 0, 60,
                180 and 300 seconds.
                                
                If a value other than a supported value is provided in a
                set request, the UPS interprets it as a the next higher
                acceptable value.  If the provided value is higher than
                the highest acceptable value, the highest acceptable
                value is used."                      8"The delay in seconds the UPS remains on after being told
                to turn off.  Allowed values are 12, 18, 24, 30, 36, 42,
                48, 54, 60, 120, 180, 240, 300, 360, 420, 480, 540, and
                600 seconds.
                                
                If a value other than a supported value is provided in a
                set request, the UPS interprets it as next higher acceptable
                value.  If the provided value is higher than the highest
                acceptable value, the highest acceptable value is used."                      D"The time in minutes for the UPS to go to 'sleep' when
                instructed.  When in sleep mode, the UPS will not provide
                output power regardless of the input line state.  Once the
                specified time has elapsed, output power will be restored.
                                
                This is a configuration setting.  The UPS will not go to
                sleep until told to do so by the manager from a management
                station.
                                
                Any input value is allowed, however the UPS only recognizes
                0 - 9999 minutes in one minute increments.
                                
                If the provided value is higher than the highest acceptable
                value, the highest acceptable value is used."                       \"WRITE: Resets the UPS EEPROM variables to default values.
                READ: returns 0"                              �"Setting this variable to turnUpsOffToConserveBattery(2)
                causes a UPS on battery to be put into 'sleep' mode immediately.
                The (10) means suspending time(10 minutes) before turn off UPS.
                UPS will turn back on when utility power is restored.
                Attempting to turn off a UPS that is not on battery will
                result in a badValue error.
                                
                Setting this value to noTurnOffUps(1) has no
                effect.
                                
                The value noTurnOffUps(1) will always be returned
                when the variable is read."                          C"Setting this variable to turnUpsOff(2) causes
                the UPS to shut off.  When in this state, the UPS
                will not provide output power regardless of the input
                line state.  The on/off switch on the UPS
                must be toggled for the UPS to return to operation.
                                
                Setting this value to noTurnUpsOff(1) has no
                effect.
                                
                The value noTurnUpsOff(1) will always be returned
                when the variable is read."                      \"Setting this variable to rebootUps(2) causes the
                UPS to shut off and turn back on.
                                
                Setting this value to noRebootUps(1) has no effect.
                                
                The value noRebootUps(1) will always be returned
                when the variable is read."                      �"Setting this variable to putUpsToSleep(2) causes
                the UPS to go to sleep for the time specified by
                upsSmartConfigUpsSleepTime.
                                
                When in sleep mode, the UPS will not provide output
                power regardless of the input line state.  Once the
                specified time has elapsed, output power will be
                restored.
                                
                Setting this value to noPutUpsToSleep(1) has no
                effect.
                                
                The value noPutUpsToSleep(1) will always be returned
                when the variable is read."                      �"Setting this variable to simulatePowerFailure(2) causes
                the UPS switch to battery power.
                                
                Setting this value to noSimulatePowerFailure(1) has no
                effect.
                                
                The value noSimulatePowerFailure(1) will always be returned
                when the variable is read."                      "UPS to identify itself by lighting all indicators
                and beeping.
                                
                Setting this variable to FlashAndBeep(2) causes
                the UPS lights all panel indicators and beeps.
                                
                Setting this value to noFlashAndBeep(1) has no
                effect.
                                
                The value noFlashAndBeep(1) will always be returned
                when the variable is read."                      �"Setting this variable to upsSmartControlTurnOnUpsLoad(2)
                causes the UPS switch back to utility power if utility
                power is present.
                                
                Setting this value to noUpsSmartControlTurnOnUpsLoad(1)
                has no effect.
                                
                The value noUpsSmartControlTurnOnUpsLoad(1) will always
                be returned when the variable is read."                                   3"The UPS system's automatic battery test schedule."                      �"Setting this variable to testDiagnostics(2) causes
                the UPS to perform a diagnostic self test.
                                
                Setting this value to noTestDiagnostics(1) has no
                effect.
                                
                The value noTestDiagnostics(1) will always be returned
                when the variable is read."                       9"The results of the last UPS diagnostics test performed."                       ["The date the last UPS diagnostics test was performed in
                mm/dd/yy format."                      "Setting this variable to testIndicators(2) causes
                the UPS to perform a front panel indicator (lights) test.
                                
                Setting this value to noTestIndicators(1) has no effect.
                                
                The value noTestIndicators(1) will always be returned
                when the variable is read."                      �"Setting this variable to performCalibration(2) causes
                the UPS to discharge to calibrate the UPS.
                The test will only start if the battery capacity is 100 percent.
                The test runs until capacity is less than 25 percent.
                                
                Setting this variable to cancelCurrentCalibration(3)
                after setting performCalibration(2) will cancel the
                current discharge.
                                
                Setting this variable to noPerformCalibration(1)
                will have no effect.
                                
                The value noPerformCalibration(1) will always be returned
                when the variable is read.
                                
                The result of the calibration will be saved in
                upsSmartTestCalibrationResult."                      �"The results of the last runtime calibration.
                                
                Value ok(1) means a successful runtime calibration.
                                
                Value invalidTest(2) indicates last calibration did
                not take place since the battery capacity was below
                100 percent.
                                
                Value calibrationInProgress(3) means a calibration
                is occurring now."                       ^"The date the last UPS runtime calibration was
                performed in mm/dd/yy format."                               "Battery Voltage"                       "Battery Capacity Percentage"                       &"Battery Time Remain (unit is Minute)"                       "Battery Electric Current"                       "Battery Temperature"                           "Input Frequency"                       "Input VoltageR"                       "Input VoltageS"                       "Input VoltageT"                           "Output Frequency"                       "Output VoltageR"                       "Output VoltageS"                       "Output VoltageT"                       "Load PercentageR"                       "Load PercentageS"                       "Load PercentageT"                           "Bypass Frequency"                       "Bypass VoltageR"                       "Bypass VoltageS"                       "Bypass VoltageT"                           "Rectifier Rotation Error"                       "Low Battery Shutdown"                       "Low Battery"                       "In And Put"                       "Back Status"                       "Charge Status"                       "Rectifier Operating"                           "Bypass Status"                       "Breaker Status"                       "AC Status"                       "Switch Mode"                       "Operating"                           "Emergency Stop"                       "High DC Shutdown"                       "Bypass Breaker"                       "Over Load"                       "Output Fail"                       "Over Temperature"                       "Short Circuit"                           "Rectifier Voltage"                       "Rectifier Frequency"                       "Bypass Voltage"                       "Bypass Frequency"                       "Output Voltage"                       "Output Frequency"                       "Battery Voltage"                       "Power Rating"                               &"The ambient environment temperature."                       "The environment humidity."                       "Security status"                       "Security status"                       "Security status"                       "Security status"                       "Security status"                       "Security status"                       "Security status"                       "Water status"                       "Smoke status"                           e"Alarm dupsAlarmOverTemperature on when the environment
                temperature over the value."                       _"Alarm dupsAlarmOverHumidity on when the environment
                humidity over the value."                       g"Alarm dupsAlarmUnderTemperature on when the environment
                temperature below the value."                       a"Alarm dupsAlarmUnderHumidity on when the environment
                humidity below the value."                               *"The number of managers to send traps to."                       "A list of managers to send traps to.  The number of
                entries is given by the value of mconfigNumTrapReceivers"                        "The managers to send traps to."                       %"The index to a trap receiver entry."                       2"The IP address of the manager to send a trap to."                       R"The community name to use in the trap when
                sent to the manager."                      "The severity threshold of traps to send to the manager.
                traps are labeled in severity as informational(1), warning(2),
                severe(3).  Only traps of equal or greater severity than
                this value are sent to the manager."                       Q"An entry will become active if yes, and will
                be deleted if no."                          