# kalwtummad2
# mini robot
<img width="578" alt="image" src="https://github.com/aelde/kalwtummad2/assets/79216582/e7074572-4a99-4716-a141-38382e5a3f9a">

**car robot control over wifi / control over hand using mqtt**\
3d : <https://collaborate.shapr3d.com/v/xkKrqxI_4FUdMemtdK0US>

โปรเจคนี้เกิดจากความสนใจใน esp32 ของผมและพอดีช่วงนั้นอาจารย์มีหุ่นยนต์ดูดฝุ่นราคาถูกที่ไม่ได้ใช้อยู่ตัวนึง และพอดีกับที่ผมมี mpu6050 อยู่ที่บ้านพอดี ผมเลยเกิดไอเดียที่จะสร้างหุ่นยนต์ที่บังคับด้วยการขยับของมือขึ้นมา
เริ่มจากการที่แกะชิ้นส่วนทั้งหมดออกแล้วก็เอาอุปกรณ์ใหม่ใส่เข้าไปแทนหมด แล้วจากนั้นก็เชื่อมอุปกรณ์ทั้งหมดกับ esp32 จนเกิดเป็นหุ่นย์นี้ขึ้นมา(kalwtummad) โดยหุ่นย์นี้จะบังคับด้วยมือผ่าน mqtt และยังสามารถบังคับผ่านหน้า dashboard ได้ด้วย

อุปกรณ์หลักๆใน project
  - esp32
  - mpu6050
  - l298n
  - lcd1602
  - I2C liquid crystal displays

การใช้งาน : หมุนมือไปในทิศที่ต้องการจะให้รถวิ่ง ดูใน HOW_TO_USE.jpeg
