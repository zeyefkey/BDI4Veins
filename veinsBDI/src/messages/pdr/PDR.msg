//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

cplusplus {{
    /** message type for pdr messages */
    static const int MANEUVER_TYPE = 22345;
}}

// General message for an arbitrary maneuver to holds common information.
// Only children of this message should be initialized.
packet PDRMessage {
    // id of the originator of this message
    int vehicleId;
    // id of the destination of this message
    int destinationId;
    // sumo external id of the sender
    string externalId;
}