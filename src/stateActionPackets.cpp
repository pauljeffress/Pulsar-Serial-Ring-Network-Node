/*
 * stateActionPackets()
 * 
 * Check global flags and if need be use global buffer etc and action whatever packet(s) have arrived.
 * 
 * There may be more than one packet that has arrived when this function is called, in fact
 *   there could be one from Left, one from Right and even one from Host. So we need to check/action
 *   all of them.
 */

#include "global_vars.h"

void stateActionPackets(){
  //Serial.println("stateActionPackets() START");
  //Serial.println("stateActionPackets() checking recHost");
  
  if (recHost)
  { 
    // This means the HOST has sent this FNIC a packet, which it must want put on th network.
    // do a quick sanity check (e.g. src addr = self, dest <> self etc etc)
    //Serial.println("actionPackets() - recHost = True");
    if((receivePacketHost.ver == SRNPV1) // correct version
      && (receivePacketHost.src == MY_ADDRESS) // Its correctly sourced.
        &&(receivePacketHost.dest != MY_ADDRESS) // I am not trying to send to myself.
          &&(receivePacketHost.packetType != PT_NULL) // the packetType field has been populated with something
            &&(receivePacketHost.copied == SRNPNOTCOPIED)) // ensure copied field set correctly.
    {
      // then send it out both the Left and Right interfaces.
      Serial.println();
      Serial.println("actionPackets() - receivedPacketHost SANE - FORWARDING Left & Right NOW...");
      Serial.println();
      STdriverLeft.sendDatum(receivePacketHost);
      mineTXLeft++; // inc counter of packets originated from this node and sent out left interface
      STdriverRight.sendDatum(receivePacketHost);
      mineTXRight++; // inc counter of packets originated from this node and sent out right interface
      //Serial.println("actionPackets() - receivedPacketHost SANE - FORWARDING COMPLETE");
    }
    else  
    {
      // The packet failed the sanity check
      Serial.println();
      Serial.println("actionPackets() - receivedPacketHost failed sanity check - SO NOT FORWARDED");
      Serial.println("Heres what it looked like");
      printPacket(receivePacketHost);

      //TODO - send error to FDR.
    }
    // packet no longer needed so empty it out ready for reuse.
    receivePacketHost = emptyPacket();
    recHost = false;  //clear the flag as we have actioned it   
  } // END - recHost



  //Serial.println("stateActionPackets() checking recLeft");
  
  if (recLeft) //i.e it came in off the ring and into our Left interface
  { 
    //Serial.println("actionPackets() - recLeft = True");
    if (receivePacketLeft.src == MY_ADDRESS)  //IF we originated it, take it off the ring e.g. drop it.
    {
      Serial.println();
      Serial.println("actionPackets() - recLeft = True AND it was sourced from us so DROPing it.");
      Serial.println();
      mineRXLeft++; // inc counter of packets originated from this node and received back in (after travelling around ring) on left interface
      // packet no longer needed so empty it out ready for reuse.
      receivePacketLeft = emptyPacket();
      recLeft = false;  //clear the flag as we have actioned it
    }   
    else if ((receivePacketLeft.dest == MY_ADDRESS) || (receivePacketLeft.dest == BROADCAST))  // we should possibly give it to our HOST
    {
      Serial.println();
      Serial.println("actionPackets() - recLeft = True AND it was NOT sourced from us AND was dest us/BCAST.");
      Serial.println();
      if (seenBefore(receivePacketLeft.src, receivePacketLeft.dest, receivePacketLeft.magicnum)) //i.e. it is a copy we have already given to our host or seen before. 
      {
        Serial.println();
        Serial.println("actionPackets() - recLeft = True AND it was NOT sourced from us AND was dest us/BCAST but we had seen it before.");
        //then just pass it on, as the originator is expecting to see it back and also if it is a BROADCAST then there may be subsequent hosts that need to see it yet.
        // Send the packet out Right interface
        Serial.print("  Sending packet out Right int, magicnum: ");Serial.println(receivePacketLeft.magicnum);
        Serial.println();
        STdriverRight.sendDatum(receivePacketLeft);
        
        // packet no longer needed so empty it out ready for reuse.
        receivePacketLeft = emptyPacket();
        recLeft = false;  //clear the flag as we have actioned it
      }
      else  // this is the first time we have seen this packet we need to pass to our host.
      {
        Serial.println();
        Serial.println("actionPackets() - recLeft = True AND it was NOT sourced from us AND was dest us/BCAST but we had NOT seen it before.");
        // Send the packet up to the HOST
        Serial.print("  Sending packet up to HOST, magicnum: ");Serial.println(receivePacketLeft.magicnum);
        sendToHost(receivePacketLeft); // determine and use correct method to send data to HOST.

        // save a record of the packet into the dedup table for later dedup checking.
        addSeenBefore(receivePacketLeft.src, receivePacketLeft.dest, receivePacketLeft.magicnum);
        // mark COPIED BYTE as SET
        receivePacketLeft.copied = SRNPCOPIED;
        // Send the packet out Right interface
        Serial.print("  Sending packet out Right int, magicnum: ");Serial.println(receivePacketLeft.magicnum);
        Serial.println();
        STdriverRight.sendDatum(receivePacketLeft);
        // packet no longer needed so empty it out ready for reuse.
        receivePacketLeft = emptyPacket();
        recLeft = false;  //clear the flag as we have actioned it
      }
    }
  } // END - recLeft



  //Serial.println("stateActionPackets() checking recRight");    

  if (recRight) //i.e it came in off the ring and into our Right interface
  { 
    //Serial.println("actionPackets() - recRight = True");
    if (receivePacketRight.src == MY_ADDRESS)  //IF we originated it, take it off the ring e.g. drop it.
    {
      Serial.println();
      Serial.println("actionPackets() - recRight = True AND it was sourced from us so DROPing it.");
      Serial.println();
      mineRXRight++; // inc counter of packets originated from this node and received back in (after travelling around ring) on left interface
      // packet no longer needed so empty it out ready for reuse.
      receivePacketRight = emptyPacket();
      recRight = false;  //clear the flag as we have actioned it
    }   
    else if ((receivePacketRight.dest == MY_ADDRESS) || (receivePacketRight.dest == BROADCAST))  // we should possibly give it to our HOST
    {
      Serial.println();
      Serial.println("actionPackets() - recRight = True AND it was NOT sourced from us AND was dest us/BCAST.");
      Serial.println();
      if (seenBefore(receivePacketRight.src, receivePacketRight.dest, receivePacketRight.magicnum)) //i.e. it is a copy we have already given to our host or seen before. 
      {
        Serial.println();
        Serial.println("actionPackets() - recRight = True AND it was NOT sourced from us AND was dest us/BCAST but we had seen it before.");
        //then just pass it on, as the originator is expecting to see it back and also if it is a BROADCAST then there may be subsequent hosts that need to see it yet.
        // Send the packet out Right interface
        Serial.print("  Sending packet out Left int, magicnum: ");Serial.println(receivePacketRight.magicnum);
        Serial.println();
        STdriverLeft.sendDatum(receivePacketRight);
        
        // packet no longer needed so empty it out ready for reuse.
        receivePacketRight = emptyPacket();
        recRight = false;  //clear the flag as we have actioned it
      }
      else  // this is the first time we have seen this packet we need to pass to our host.
      {
        Serial.println();
        Serial.println("actionPackets() - recRight = True AND it was NOT sourced from us AND was dest us/BCAST but we had NOT seen it before.");
        // Send the packet up to the HOST
        Serial.print("  Sending packet up to HOST, magicnum: ");Serial.println(receivePacketRight.magicnum);
        sendToHost(receivePacketRight); // determine and use correct method to send data to HOST.
        
        // save a record of the packet into the dedup table for later dedup checking.
        addSeenBefore(receivePacketRight.src, receivePacketRight.dest, receivePacketRight.magicnum);
        // mark COPIED BYTE as SET
        receivePacketRight.copied = SRNPCOPIED;
        // Send the packet out Left interface
        Serial.print("  Sending packet out Left int, magicnum: ");Serial.println(receivePacketRight.magicnum);
        Serial.println();
        STdriverLeft.sendDatum(receivePacketRight);
        // packet no longer needed so empty it out ready for reuse.
        receivePacketRight = emptyPacket();
        recRight = false;  //clear the flag as we have actioned it
      }
    }
  } // END - recRight

  state = HOUSEKEEPING;  // always goto that state after this one.  

  //Serial.println("stateActionPackets() END");
}   // END - stateActionPackets()