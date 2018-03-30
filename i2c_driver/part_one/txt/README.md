# i2c Commands

write:
- Writes to the control register and sets the extended mode to on. 
```
i2c read 0x90 0x00 1
```

read:
- Read the temperature register
```
i2c write 0x90 0x01 0x60B0
```
