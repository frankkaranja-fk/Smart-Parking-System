Module 1; Slot availability
1. Start
2. Connect to the parking database
3. Count the number of slots where status = "Available"
4. Display the total available slots on the screen/board
5. Optionally show a map/grid of free and occupied slots
6. End

Module 2:Vehicle registration
1. Start
2. Read vehicle registration number (number plate)
3. Get current system time as EntryTime
4. Check if the vehicle is already parked (optional security check)
5. If vehicle is new:
 - Create a new parking record
 - Store: NumberPlate, EntryTime, Status = "Parked"
6. Call Slot Allocation Module
7. End

Module 3:Parking Slot Allocation
Algorithm AllocateSlot
1. Start
2. Search for the first slot where status = "Available"
3. If a free slot is found:
 - Assign the SlotID to the current vehicle
 - Update slot status to "Occupied"
 - Update vehicle record with SlotID
4. Else:
 - Display "Parking Full"
5. End

Module 4:Fee Calculation
Algorithm CalculateFee
1. Start
2. Read vehicle registration number
3. Retrieve EntryTime from database
4. Get current system time as ExitTime
5. Calculate Duration = ExitTime – EntryTime (in minutes)
6. Apply fee rules:
 If Duration ≤ 30 minutes → Fee = 0
 Else if Duration ≤ 120 minutes → Fee = 50
 Else if Duration ≤ 240 minutes → Fee = 100
 Else if Duration ≤ 360 minutes → Fee = 300
 Else → Fee = 500
7. Display Duration and Fee to the driver
8. Return Fee
9. End

Module 5:Payment and Barrier control
Algorithm ProcessPaymentAndExit
1. Start
2. Call CalculateFee Module → get Fee
3. Accept payment from driver
4. If payment is successful:
 - Update vehicle record: ExitTime, FeePaid, Status = "Exited"
 - Free the allocated slot (set status = "Available")
 - Open the exit barrier
5. Else:
 - Display "Payment Failed – Barrier remains closed"
6. End