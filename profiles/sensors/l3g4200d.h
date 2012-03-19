/**************************************************************************
    PygmyOS ( Pygmy Operating System )
    Copyright (C) 2011-2012  Warren D Greenway

    This file is part of PygmyOS.

    PygmyOS is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PygmyOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with PygmyOS.  If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/

#define L3G4200D_ADDRESS_0                  0x68 // Must be left shifted for Read/Write
#define L3G4200D_ADDRESS_1                  0x69 // 
#define L3G4200D_WHOAMI                     0x0F
#define L3G4200D_CTRL_REG1                  0x20
#define L3G4200D_CTRL_REG2                  0x21
#define L3G4200D_CTRL_REG3                  0x22
#define L3G4200D_CTRL_REG4                  0x23
#define L3G4200D_CTRL_REG5                  0x24
#define L3G4200D_REFERENCE                  0x25
#define L3G4200D_OUT_TEMP                   0x26
#define L3G4200D_STATUS                     0x27
#define L3G4200D_OUT_X_L                    0x28
#define L3G4200D_OUT_X_H                    0x29
#define L3G4200D_OUT_Y_L                    0x2A
#define L3G4200D_OUT_Y_H                    0x2B
#define L3G4200D_OUT_Z_L                    0x2C
#define L3G4200D_OUT_Z_H                    0x2D
#define L3G4200D_FIFO_CTRL                  0x2E
#define L3G4200D_SRC                        0x2F
#define L3G4200D_INT1_CFG                   0x30
#define L3G4200D_INT1_SRC                   0x31
#define L3G4200D_INT1_TSH_XH                0x32
#define L3G4200D_INT1_TSH_XL                0x33
#define L3G4200D_INT1_TSH_YH                0x34
#define L3G4200D_INT1_TSH_YL                0x35
#define L3G4200D_INT1_TSH_ZH                0x36
#define L3G4200D_INT1_TSH_ZL                0x37
#define L3G4200D_INT1_DURATION              0x38
// CTRL_REG1
#define L3G4200D_DATARATE_MASK              0xC0
#define L3G4200D_DATARATE_100HZ             0x00
#define L3G4200D_DATARATE_200HZ             0x40
#define L3G4200D_DATARATE_400HZ             0x80
#define L3G4200D_DATARATE_800HZ             0xC0
#define L3G4200D_BANDWIDTH_MASK             0x30
#define L3G4200D_BANDWIDTH_12P5             0x00
#define L3G4200D_BANDWIDTH_25               0x10
#define L3G4200D_BANDWIDTH_50               0x20
#define L3G4200D_BANDWIDTH_70               0x30
#define L3G4200D_PD                         0x08
#define L3G4200D_ZEN                        0x04
#define L3G4200D_YEN                        0x02
#define L3G4200D_XEN                        0x01
// CTRL_REG2, High Pass Mode, High Pass Cut-Off Frequency
#define L3G4200D_HPM_MASK                   0x30
#define L3G4200D_HPM_NORMAL                 0x00
#define L3G4200D_HPM_REFERENCE              0x10
#define L3G4200D_HPM_NORMAL1                0x20
#define L3G4200D_HPM_AUTORESET              0x30
#define L3G4200D_HPCF_MASK                  0x0F
#define L3G4200D_HPCF_0                     0x00
#define L3G4200D_HPCF_1                     0x01
#define L3G4200D_HPCF_2                     0x02
#define L3G4200D_HPCF_3                     0x03
#define L3G4200D_HPCF_4                     0x04
#define L3G4200D_HPCF_5                     0x05
#define L3G4200D_HPCF_6                     0x06
#define L3G4200D_HPCF_7                     0x07
#define L3G4200D_HPCF_8                     0x08
#define L3G4200D_HPCF_9                     0x09
// CTRL_REG3
#define L3G4200D_I1_INT1                    0x80
#define L3G4200D_I1_BOOT                    0x40
#define L3G4200D_H_LACTIVE                  0x20
#define L3G4200D_PP_OD                      0x10
#define L3G4200D_I2_DRDY                    0x08
#define L3G4200D_I2_WTM                     0x04
#define L3G4200D_I2_ORUN                    0x02
#define L3G4200D_I2_EMPTY                   0x01
// CTRL_REG4
#define L3G4200D_BDU                        0x80
#define L3G4200D_BLE                        0x40
#define L3G4200D_FS_MASK                    0x30
#define L3G4200D_FS_250                     0x00
#define L3G4200D_FS_500                     0x10
#define L3G4200D_FS_2000                    0x20
#define L3G4200D_ST_MASK                    0x06
#define L3G4200D_ST_0                       0x02
#define L3G4200D_ST_1                       0x06
#define L3G4200D_SIM                        0x01
// CTRL_REG5
#define L3G4200D_BOOT                       0x80
#define L3G4200D_FIFO_EN                    0x40
#define L3G4200D_HPEN                       0x10
#define L3G4200D_INT1_MASK                  0x0C
#define L3G4200D_INT1_NORMAL                0x00
#define L3G4200D_INT1_HP                    0x04
#define L3G4200D_INT1_LP                    0x08 // LP and HPEN set HPLP mode
#define L3G4200D_OUT_MASK                   0x03
#define L3G4200D_OUT_NORMAL                 0x00
#define L3G4200D_OUT_HP                     0x01
#define L3G4200D_OUT_LP                     0x02 // LP and HPEN set HPLP mode
// STATUS_REG
#define L3G4200D_ZYXOR                      0x80 // XYX Axis Overrun
#define L3G4200D_ZOR                        0x40 // Z Axis Overrun
#define L3G4200D_YOR                        0x20 // Y Axis Overrun
#define L3G4200D_XOR                        0x10 // X Axis Overrun
#define L3G4200D_ZYXDA                      0x08 // XYZ Axis New Data Available
#define L3G4200D_ZDA                        0x04 // Z Axis New Data Available
#define L3G4200D_YDA                        0x02 // Y Axis New Data Available
#define L3G4200D_XDA                        0x01 // X Axis New Data Available
// FIFO_CTRL_REG
#define L3G4200D_FIFO_CTRL_MASK             0xE0
#define L3G4200D_FIFO_CTRL_BYPASS           0x00
#define L3G4200D_FIFO_CTRL_FIFOMODE         0x20
#define L3G4200D_FIFO_CTRL_STREAM           0x40
#define L3G4200D_FIFO_CTRL_STREAMTOFIFO     0x60
#define L3G4200D_FIFO_CTRL_BYPASSTOSTREAM   0x80
#define L3G4200D_FIFO_CTRL_WTM_MASK         0x1F
// FIFO_SRC_REG
#define L3G4200D_FIFO_SRC_WTM               0x80
#define L3G4200D_FIFO_SRC_OVRN              0x40
#define L3G4200D_FIFO_SRC_EMPTY             0x20
#define L3G4200D_FIFO_SRC_FSS_MASK          0x1F // FIFO Stored Data Level
//INT1_CFG
#define L3G4200D_INT1_ANDOR                 0x80 
#define L3G4200D_INT1_LIR                   0x40 // Latch Interrupt Request
#define L3G4200D_INT1_ZHIE                  0x20 // Enable Interrupt on Z High
#define L3G4200D_INT1_ZLIE                  0x10 // Enable Interrupt on Z Low
#define L3G4200D_INT1_YHIE                  0x08 // Enable Interrupt on Y High
#define L3G4200D_INT1_YLIE                  0x04 // Enable Interrupt on Y Low
#define L3G4200D_INT1_XHIE                  0x02 // Enable Interrupt on X High
#define L3G4200D_INT1_XLIE                  0x01 // Enable Interrupt on X Low
// INT1_SRC
#define L3G4200D_INT1_IA                    0x40 // Interrupt Active
#define L3G4200D_INT1_ZH                    0x20 // Z High
#define L3G4200D_INT1_ZL                    0x10 // Z Low
#define L3G4200D_INT1_YH                    0x08 // Y High
#define L3G4200D_INT1_YL                    0x04 // Y Low
#define L3G4200D_INT1_XH                    0x02 // X High
#define L3G4200D_INT1_XL                    0x01 // X Low

