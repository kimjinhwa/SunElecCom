 �-- Changes to rfc1907 - SNMPv2-MIB (object for SNMPv2 protocol)
--      NOTE: the following should be in a group -
--            snmpTrapEnterprise, snmpTrapOID, and
--            warmStart!
-- dperkins@scruznet.com
                                                                             %"The MIB module for SNMPv2 entities.""        Marshall T. Rose

             Postal: Dover Beach Consulting, Inc.
                     420 Whisman Court
                     Mountain View, CA  94043-2186
                     US

                Tel: +1 415 968 1052

             E-mail: mrose@dbc.mtview.ca.us" "9304010000Z" Q"The initial revision of this MIB module was published as
            RFC 1450."                       �"The authoritative identification of the notification
            currently being sent.  This variable occurs as the second
            varbind in every SNMPv2-Trap-PDU and InformRequest-PDU."                      "The authoritative identification of the enterprise
            associated with the trap currently being sent.  When a
            SNMPv2 proxy agent is mapping an RFC1157 Trap-PDU into a
            SNMPv2-Trap-PDU, this variable occurs as the last varbind."                           �"A coldStart trap signifies that the SNMPv2 entity, acting
            in an agent role, is reinitializing itself and that its
            configuration may have been altered."                 �"A warmStart trap signifies that the SNMPv2 entity, acting
            in an agent role, is reinitializing itself such that its
            configuration is unaltered."                �"An authenticationFailure trap signifies that the SNMPv2
            entity, acting in an agent role, has received a protocol
            message that is not properly authenticated.  While all
            implementations of the SNMPv2 must be capable of generating
            this trap, the snmpEnableAuthenTraps object indicates
            whether this trap will be generated."                    �"An advisory lock used to allow several cooperating SNMPv2
            entities, all acting in a manager role, to coordinate their
            use of the SNMPv2 set operation.

            This object is used for coarse-grain coordination.  To
            achieve fine-grain coordination, one or more similar objects
            might be defined within each MIB group, as appropriate."                               ["The compliance statement for SNMPv2 entities which
            implement the SNMPv2 MIB."   h"This group is mandatory for SNMPv2 entities which
            support community-based authentication."                 g"A collection of objects providing basic instrumentation and
            control of an SNMPv2 entity."                 �"A collection of objects providing basic instrumentation of
            a SNMPv2 entity which supports community-based
            authentication."                 �"A collection of objects which allow several cooperating
            SNMPv2 entities, all acting in a manager role, to coordinate
            their use of the SNMPv2 set operation."                 X"The system group defines objects which are common to all
            managed systems."                 U"The two notifications which an SNMPv2 entity is required to
            implement."                     O"A collection of objects from RFC 1213 made obsolete by this
            MIB."                             �"A textual description of the entity.  This value should
            include the full name and version identification of the
            system's hardware type, software operating-system, and
            networking software."                      "The vendor's authoritative identification of the network
            management subsystem contained in the entity.  This value is
            allocated within the SMI enterprises subtree (1.3.6.1.4.1)
            and provides an easy and unambiguous means for determining
            `what kind of box' is being managed.  For example, if vendor
            `Flintstones, Inc.' was assigned the subtree
            1.3.6.1.4.1.4242, it could assign the identifier
            1.3.6.1.4.1.4242.1.1 to its `Fred Router'."                       "The time (in hundredths of a second) since the network
            management portion of the system was last re-initialized."                       �"The textual identification of the contact person for this
            managed node, together with information on how to contact
            this person.  If no contact information is known, the value
            is the zero-length string."                       �"An administratively-assigned name for this managed node.
            By convention, this is the node's fully-qualified domain
            name.  If the name is unknown, the value is the zero-length
            string."                       �"The physical location of this node (e.g., `telephone
            closet, 3rd floor').  If the location is unknown, the value
            is the zero-length string."                      �"A value which indicates the set of services that this
            entity may potentially offers.  The value is a sum.  This
            sum initially takes the value zero, Then, for each layer, L,
            in the range 1 through 7, that this node performs
            transactions for, 2 raised to (L - 1) is added to the sum.
            For example, a node which performs only routing functions
            would have a value of 4 (2^(3-1)).  In contrast, a node
            which is a host offering application services would have a
            value of 72 (2^(4-1) + 2^(7-1)).  Note that in the context
            of the Internet suite of protocols, values should be
            calculated accordingly:

                 layer      functionality
                   1        physical (e.g., repeaters)
                   2        datalink/subnetwork (e.g., bridges)
                   3        internet (e.g., supports the IP)
                   4        end-to-end  (e.g., supports the TCP)
                   7        applications (e.g., supports the SMTP)

            For systems including OSI protocols, layers 5 and 6 may also
            be counted."                       y"The value of sysUpTime at the time of the most recent
            change in state or value of any instance of sysORID."                      A"The (conceptual) table listing the capabilities of the
            local SNMPv2 entity acting in an agent role with respect to
            various MIB modules.  SNMPv2 entities having dynamically-
            configurable support of MIB modules will have a
            dynamically-varying number of conceptual rows."                       ."An entry (conceptual row) in the sysORTable."                       o"The auxiliary variable used for identifying instances of
            the columnar objects in the sysORTable."                       �"An authoritative identification of a capabilities statement
            with respect to various MIB modules supported by the local
            SNMPv2 entity acting in an agent role."                       m"A textual description of the capabilities identified by the
            corresponding instance of sysORID."                       \"The value of sysUpTime at the time this conceptual row was
            last instanciated."                           d"The total number of messages delivered to the SNMP entity
            from the transport service."                       �"The total number of SNMP messages which were delivered to
            the SNMP entity and were for an unsupported SNMP version."                       �"The total number of SNMP messages delivered to the SNMP
            entity which used a SNMP community name not known to said
            entity."                       �"The total number of SNMP messages delivered to the SNMP
            entity which represented an SNMP operation which was not
            allowed by the SNMP community named in the message."                       {"The total number of ASN.1 or BER errors encountered by the
            SNMP entity when decoding received SNMP messages."                      �"Indicates whether the SNMP entity is permitted to generate
            authenticationFailure traps.  The value of this object
            overrides any configuration information; as such, it
            provides a means whereby all authenticationFailure traps may
            be disabled.

            Note that it is strongly recommended that this object be
            stored in non-volatile memory so that it remains constant
            across re-initializations of the network management system."                      �"The total number of GetRequest-PDUs, GetNextRequest-PDUs,
            GetBulkRequest-PDUs, SetRequest-PDUs, and InformRequest-PDUs
            delivered to the SNMP entity which were silently dropped
            because the size of a reply containing an alternate
            Response-PDU with an empty variable-bindings field was
            greater than either a local constraint or the maximum
            message size associated with the originator of the request."                      �"The total number of GetRequest-PDUs, GetNextRequest-PDUs,
            GetBulkRequest-PDUs, SetRequest-PDUs, and InformRequest-PDUs
            delivered to the SNMP entity which were silently dropped
            because the transmission of the (possibly translated)
            message to a proxy target failed in a manner (other than a
            time-out) such that no Response-PDU could be returned."                       �"The total number of SNMP Messages which were
            passed from the SNMP protocol entity to the
            transport service."                       �"The total number of SNMP PDUs which were
            delivered to the SNMP protocol entity and for
            which the value of the error-status field is
            `tooBig'."                       �"The total number of SNMP PDUs which were
            delivered to the SNMP protocol entity and for
            which the value of the error-status field is
            `noSuchName'."                       �"The total number of SNMP PDUs which were
            delivered to the SNMP protocol entity and for
            which the value of the error-status field is
            `badValue'."                      �"The total number valid SNMP PDUs which were
            delivered to the SNMP protocol entity and for
            which the value of the error-status field is
            `readOnly'.  It should be noted that it is a
            protocol error to generate an SNMP PDU which
            contains the value `readOnly' in the error-status
            field, as such this object is provided as a means
            of detecting incorrect implementations of the
            SNMP."                       �"The total number of SNMP PDUs which were
            delivered to the SNMP protocol entity and for
            which the value of the error-status field is
            `genErr'."                       �"The total number of MIB objects which have been
            retrieved successfully by the SNMP protocol entity
            as the result of receiving valid SNMP Get-Request
            and Get-Next PDUs."                       �"The total number of MIB objects which have been
            altered successfully by the SNMP protocol entity
            as the result of receiving valid SNMP Set-Request
            PDUs."                       �"The total number of SNMP Get-Request PDUs which
            have been accepted and processed by the SNMP
            protocol entity."                       �"The total number of SNMP Get-Next PDUs which have
            been accepted and processed by the SNMP protocol
            entity."                       �"The total number of SNMP Set-Request PDUs which
            have been accepted and processed by the SNMP
            protocol entity."                       �"The total number of SNMP Get-Response PDUs which
            have been accepted and processed by the SNMP
            protocol entity."                       �"The total number of SNMP Trap PDUs which have
            been accepted and processed by the SNMP protocol
            entity."                       �"The total number of SNMP PDUs which were
            generated by the SNMP protocol entity and for
            which the value of the error-status field is
            `tooBig.'"                       �"The total number of SNMP PDUs which were
            generated by the SNMP protocol entity and for
            which the value of the error-status is
            `noSuchName'."                       �"The total number of SNMP PDUs which were
            generated by the SNMP protocol entity and for
            which the value of the error-status field is
            `badValue'."                       �"The total number of SNMP PDUs which were
            generated by the SNMP protocol entity and for
            which the value of the error-status field is
            `genErr'."                       o"The total number of SNMP Get-Request PDUs which
            have been generated by the SNMP protocol entity."                       l"The total number of SNMP Get-Next PDUs which have
            been generated by the SNMP protocol entity."                       o"The total number of SNMP Set-Request PDUs which
            have been generated by the SNMP protocol entity."                       p"The total number of SNMP Get-Response PDUs which
            have been generated by the SNMP protocol entity."                       h"The total number of SNMP Trap PDUs which have
            been generated by the SNMP protocol entity."                          