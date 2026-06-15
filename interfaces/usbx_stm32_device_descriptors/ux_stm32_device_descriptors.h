/**
********************************************************************************
* file           : ux_stm32_device_descriptors.h
* brief          : Header for ux_stm32_device_descriptors.c
*
********************************************************************************
* Copyright (c) 2025-2026 STMicroelectronics.
*
* This program and the accompanying materials are made available under the
* terms of the MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef UX_STM32_DEVICE_DESCRIPTORS_H
#define UX_STM32_DEVICE_DESCRIPTORS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ux_api.h"
#include "cmsis_compiler.h"

#ifdef USBD_INCLUDE_USER_DESC_CONFIG_FILE

#include "ux_stm32_device_descriptors_config.h"

#endif /* USBD_INCLUDE_USER_DESC_CONFIG_FILE */

#if ((USBD_HID_MOUSE_ACTIVATED == 1U) || (USBD_HID_KEYBOARD_ACTIVATED == 1U) || (USBD_HID_CUSTOM_ACTIVATED == 1U))
#define USBD_HID_CLASS_ACTIVATED        1U
#endif

/* Private includes ----------------------------------------------------------*/
#if USBD_HID_CLASS_ACTIVATED == 1U
#include "ux_device_class_hid.h"
#endif
#if USBD_CDC_ACM_CLASS_ACTIVATED == 1U
#include "ux_device_class_cdc_acm.h"
#endif
#if USBD_MSC_CLASS_ACTIVATED == 1U
#include "ux_device_class_storage.h"
#endif
#if USBD_DFU_CLASS_ACTIVATED == 1U
#include "ux_device_class_dfu.h"
#endif
#if USBD_AUDIO_CLASS_ACTIVATED == 1U
#include "ux_device_class_audio.h"
#endif
#if USBD_VIDEO_CLASS_ACTIVATED == 1U
#include "ux_device_class_video.h"
#endif
#if USBD_PRINTER_CLASS_ACTIVATED == 1U
#include "ux_device_class_printer.h"
#endif
#if USBD_CCID_CLASS_ACTIVATED == 1U
#include "ux_device_class_ccid.h"
#endif
#if USBD_MTP_CLASS_ACTIVATED == 1U
#include "ux_device_class_pima.h"
#endif

/* Private defines -----------------------------------------------------------*/

#ifndef USBD_MAX_SUPPORTED_CLASS
#define USBD_MAX_SUPPORTED_CLASS                0x03U
#endif

#ifndef USBD_MAX_CLASS_INTERFACES
#define USBD_MAX_CLASS_INTERFACES               0x04U
#endif

#ifndef USBD_MAX_SUPPORTED_USER_STRING_DESC
#define USBD_MAX_SUPPORTED_USER_STRING_DESC     0x07U
#endif

#ifndef USBD_COMPOSITE_USE_IAD
#define USBD_COMPOSITE_USE_IAD                  0x00U
#endif


#ifndef USB_DESC_TYPE_IAD
#define USB_DESC_TYPE_IAD                  0x0BU
#endif

/* USB device class type ENUM */
typedef enum
{
  USBD_CLASS_TYPE_NONE     = 0,
  USBD_CLASS_TYPE_DFU      = 1,
  USBD_CLASS_TYPE_HID      = 2,
  USBD_CLASS_TYPE_MSC      = 3,
  USBD_CLASS_TYPE_CDC_ACM  = 4,
  USBD_CLASS_TYPE_CDC_ECM  = 5,
  USBD_CLASS_TYPE_RNDIS    = 6,
  USBD_CLASS_TYPE_PIMA_MTP = 7,
  USBD_CLASS_TYPE_CCID     = 8,
  USBD_CLASS_TYPE_PRINTER  = 9,
  USBD_CLASS_TYPE_VIDEO    = 10,
  USBD_CLASS_TYPE_AUDIO_10 = 11,
  USBD_CLASS_TYPE_AUDIO_20 = 12,
}USB_DEVICE_ClASS_TYPE;

/* USB device endpoint handle structure */
typedef struct  USB_STRING_DESC_HANDLE_STRUCT
{
  USHORT        language_id;
  UCHAR         string_index;
  struct USB_STRING_DESC_HANDLE_STRUCT     *next_string_desc;
} USB_STRING_DESC_HANDLE;

/* USB device endpoint handle structure */
typedef struct  USB_DEVICE_ENDPOINT_HANDLE_STRUCT
{
  UCHAR         endpoint_add;
  UCHAR         endpoint_type;
  USHORT        endpoint_size_fs;
  UCHAR         endpoint_size_binterval_fs;
#if USBD_HIGH_SPEED_SUPPORTED == 1
  USHORT        endpoint_size_hs;
  UCHAR         endpoint_size_binterval_hs;
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  struct USB_DEVICE_ENDPOINT_HANDLE_STRUCT     *next_endpoint;
} USB_DEVICE_ENDPOINT_HANDLE;


/* USB device Interface handle structure */
typedef struct  USB_DEVICE_INTERFACE_HANDLE_STRUCT
{
  UCHAR                         interface_numbers;
  UCHAR                         alternate_setting;
  UCHAR                         endpoints_numbers;
  UCHAR                         interface_class;
  UCHAR                         interface_subclass;
  UCHAR                         interface_protocol;
  UCHAR                         i_interface;
  ULONG                         other_parameter;
  USB_DEVICE_ENDPOINT_HANDLE    *device_endpoint;
  struct USB_DEVICE_INTERFACE_HANDLE_STRUCT     *next_interface;
  struct USB_DEVICE_INTERFACE_HANDLE_STRUCT     *next_alt_setting;
}USB_DEVICE_INTERFACE_HANDLE;


/* USB Composite handle structure */
typedef struct
{
  UCHAR                         device_class_id;
  USB_DEVICE_ClASS_TYPE         device_class_type;
  USB_DEVICE_INTERFACE_HANDLE   *interfaces;
} USB_DEVICE_COMPOSITE_HANDLE;


/* USB Device handle structure */
typedef struct
{
  UCHAR                         device_class_numbers;
  ULONG                         device_class_total_itfs_num;
  USB_DEVICE_COMPOSITE_HANDLE   device_class_list[USBD_MAX_SUPPORTED_CLASS];
} USB_DEVICE_HANDLE;


/* USB device endpoint handle structure */
typedef struct
{
  USHORT        language_id;
  UCHAR         string_index;
  UCHAR         string_length;
} __PACKED USB_STRING_DESC;

/* USB device endpoint handle structure */
typedef struct
{
  UCHAR         is_used;
  USHORT        language_id;
  UCHAR         string_index;
  UCHAR         string_length;
  UCHAR         *string_descriptor;
} __PACKED USBD_USER_STRING_DESC;

/* USB Device handle structure */
typedef struct
{
  UCHAR                 user_string_desc_numbers;
  USBD_USER_STRING_DESC user_string_desc_list[USBD_MAX_SUPPORTED_USER_STRING_DESC];
} USBD_USER_STRING_DESC_HANDLE;


/* USB language id descriptors structure */
typedef struct
{
  USHORT        language_id;
} __PACKED USB_DEVICE_LANGUAGE_ID_DESC;


/* USB device framework handle structure */
typedef struct
{
  UCHAR *framework;
  ULONG framework_length;
} USBD_FRAMEWORK_HANDLE;

typedef struct
{
  USBD_FRAMEWORK_HANDLE   device_high_speed;
  USBD_FRAMEWORK_HANDLE   device_full_speed;
  USBD_FRAMEWORK_HANDLE   string;
  USBD_FRAMEWORK_HANDLE   languageid;
} USB_DESCRIPTOR;


/* USB Device descriptors structure */
typedef struct
{
  UCHAR         bLength;
  UCHAR         bDescriptorType;
  USHORT        bcdUSB;
  UCHAR         bDeviceClass;
  UCHAR         bDeviceSubClass;
  UCHAR         bDeviceProtocol;
  UCHAR         bMaxPacketSize0;
  USHORT        idVendor;
  USHORT        idProduct;
  USHORT        bcdDevice;
  UCHAR         iManufacturer;
  UCHAR         iProduct;
  UCHAR         iSerialNumber;
  UCHAR         bNumConfigurations;
} __PACKED USB_DEVICE_DESC;

/* USB Qualifier descriptors structure */
typedef struct
{
  UCHAR         bLength;
  UCHAR         bDescriptorType;
  USHORT        bcdUSB;
  UCHAR         bDeviceClass;
  UCHAR         bDeviceSubClass;
  UCHAR         bDeviceProtocol;
  UCHAR         bMaxPacketSize0;
  UCHAR         bNumConfigurations;
  UCHAR         bReserved;
} __PACKED USBD_QUALIFIER_DESC;


/* USB Config descriptors structure */
typedef struct
{
  UCHAR         bLength;
  UCHAR         bDescriptorType;
  USHORT        wTotalLength;
  UCHAR         bNumInterfaces;
  UCHAR         bConfigurationValue;
  UCHAR         iConfiguration;
  UCHAR         bmAttributes;
  UCHAR         bMaxPower;
} __PACKED USBD_CONFIGURATION_DESC;


/* USB interface descriptors structure */
typedef struct
{
  UCHAR         bLength;
  UCHAR         bDescriptorType;
  UCHAR         bInterfaceNumber;
  UCHAR         bAlternateSetting;
  UCHAR         bNumEndPoints;
  UCHAR         bInterfaceClass;
  UCHAR         bInterfaceSubClass;
  UCHAR         bInterfaceProtocol;
  UCHAR         iInterface;
} __PACKED USBD_INTERFACE_DESC;


/* USB endpoint descriptors structure */
typedef struct
{
  UCHAR         bLength;
  UCHAR         bDescriptorType;
  UCHAR         bEndpointAddress;
  UCHAR         bmAttributes;
  USHORT        wMaxPacketSize;
  UCHAR         bInterval;
} __PACKED USBD_ENDPOINT_DESC;


/* USB IAD descriptors structure */
typedef struct
{
  UCHAR         bLength;
  UCHAR         bDescriptorType;
  UCHAR         bFirstInterface;
  UCHAR         bInterfaceCount;
  UCHAR         bFunctionClass;
  UCHAR         bFunctionSubClass;
  UCHAR         bFunctionProtocol;
  UCHAR         iFunction;
} __PACKED USBD_IAD_DESC;


#if USBD_DFU_CLASS_ACTIVATED == 1U
typedef struct
{
  UCHAR         bLength;
  UCHAR         bDescriptorType;
  UCHAR         bmAttributes;
  USHORT        wDetachTimeOut;
  USHORT        wTransferSize;
  USHORT        bcdDFUVersion;
} __PACKED USBD_DFU_FUNCTIONAL_DESC;
#endif /* USBD_DFU_CLASS_ACTIVATED */


#if USBD_AUDIO_CLASS_ACTIVATED == 1U
typedef struct
{
  UCHAR         bLength;
  UCHAR         bDescriptorType;
  UCHAR         bDescriptorSubtype;
  USHORT        bcdADC;
  UCHAR         bCategory;
  USHORT        wTotalLength;
  UCHAR         bmControls;
} __PACKED USBD_AUDIO_AC_CS_IF_DESC;

typedef struct
{
  UCHAR         bLength;
  UCHAR         bDescriptorType;
  UCHAR         bDescriptorSubtype;
  UCHAR         bClockID;
  UCHAR         bmAttributes;
  UCHAR         bmControls;
  UCHAR         bAssocTerminal;
  UCHAR         iClockSource;
} __PACKED USBD_AUDIO_CLOCK_SOURCE_DESC;

typedef struct
{
  UCHAR         bLength;
  UCHAR         bDescriptorType;
  UCHAR         bDescriptorSubtype;
  UCHAR         bTerminalID;
  USHORT        wTerminalType;
  UCHAR         bAssocTerminal;
  UCHAR         bCSourceID;
  UCHAR         bNrChannels;
  ULONG         bmChannelConfig;
  UCHAR         iChannelNames;
  USHORT        bmControls;
  UCHAR         iTerminal;
} __PACKED USBD_AUDIO_INPUT_TERMINAL_DESC;

typedef struct
{
  UCHAR         bLength;
  UCHAR         bDescriptorType;
  UCHAR         bDescriptorSubtype;
  UCHAR         bUnitID;
  UCHAR         bSourceID;
  ULONG         bmaControls[3];
  UCHAR         iFeature;
} __PACKED USBD_AUDIO_FEATURE_UNIT_DESC;

typedef struct
{
  UCHAR         bLength;
  UCHAR         bDescriptorType;
  UCHAR         bDescriptorSubtype;
  UCHAR         bTerminalID;
  USHORT        wTerminalType;
  UCHAR         bAssocTerminal;
  UCHAR         bSourceID;
  UCHAR         bCSourceID;
  USHORT        bmaControls;
  UCHAR         iTerminal;
} __PACKED USBD_AUDIO_OUTPUT_TERMINAL_DESC;

typedef struct
{
  UCHAR         bLength;
  UCHAR         bDescriptorType;
  UCHAR         bDescriptorSubtype;
  UCHAR         bTerminalLink;
  UCHAR         bmControls;
  UCHAR         bFormatType;
  ULONG         bmFormats;
  UCHAR         bNrChannels;
  ULONG         bmChannelConfig;
  UCHAR         iChannelNames;
} __PACKED USBD_AUDIO_AS_CS_IF_DESC;

typedef struct
{
  UCHAR         bLength;
  UCHAR         bDescriptorType;
  UCHAR         bDescriptorSubtype;
  UCHAR         bFormatType;
  UCHAR         bSubslotSize;
  UCHAR         bBitResolution;
} __PACKED USBD_AUDIO_FORMAT_TYPE_DESC;

typedef struct
{
  UCHAR         bLength;
  UCHAR         bDescriptorType;
  UCHAR         bDescriptorSubtype;
  UCHAR         bmAttributes;
  UCHAR         bmControls;
  UCHAR         bLockDelayUnits;
  USHORT        wLockDelay;
} __PACKED USBD_AUDIO_CS_EP_DESC;
#endif /* USBD_AUDIO_CLASS_ACTIVATED */


#if USBD_CCID_CLASS_ACTIVATED == 1U
typedef struct
{
  UCHAR         bLength;
  UCHAR         bDescriptorType;
  USHORT        bcdCCID;
  UCHAR         bMaxSlotIndex;
  UCHAR         bVoltageSupport;
  ULONG         dwProtocols;
  ULONG         dwDefaultClock;
  ULONG         dwMaximumClock;
  UCHAR         bNumClockSupported;
  ULONG         dwDataRate;
  ULONG         dwMaxDataRate;
  UCHAR         bNumDataRatesSupported;
  ULONG         dwMaxIFSD;
  ULONG         dwSynchProtocols;
  ULONG         dwMechanical;
  ULONG         dwFeatures;
  ULONG         dwMaxCCIDMessageLength;
  UCHAR         bClassGetResponse;
  UCHAR         bClassEnvelope;
  USHORT        wLcdLayout;
  UCHAR         bPINSupport;
  UCHAR         bMaxCCIDBusySlots;
} __PACKED USBD_CCID_DESC;
#endif /* USBD_CCID_CLASS_ACTIVATED */


#if USBD_HID_CLASS_ACTIVATED == 1U

/* Enum HID Interface Type */
typedef enum
{
  USBD_HID_NONE_INTERFACE       = 0,
  USBD_HID_KEYBOARD_INTERFACE   = 1,
  USBD_HID_MOUSE_INTERFACE      = 2,
  USBD_HID_CUSTOM_INTERFACE     = 3,
} USBD_HID_INTERFACE_TYPE;

/* USB HID descriptors structure */
typedef struct
{
  UCHAR         bLength;
  UCHAR         bDescriptorType;
  USHORT        bcdHID;
  UCHAR         bCountryCode;
  UCHAR         bNumDescriptors;
  UCHAR         bHIDDescriptorType;
  USHORT        wDescriptorLength;
} __PACKED USBD_HID_DESC;

#endif /* USBD_HID_CLASS_ACTIVATED */


#if USBD_CDC_ACM_CLASS_ACTIVATED == 1U

typedef struct
{
  /* Header Functional Descriptor*/
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bDescriptorSubtype;
  uint16_t bcdCDC;
} __PACKED USBD_CDC_HEADER_DESC;

typedef struct
{
  /* Call Management Functional Descriptor*/
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bDescriptorSubtype;
  uint8_t bmCapabilities;
  uint8_t bDataInterface;
} __PACKED USBD_CDC_CALL_MANAGEMENT_DESC;

typedef struct
{
  /* ACM Functional Descriptor*/
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bDescriptorSubtype;
  uint8_t bmCapabilities;
} __PACKED USBD_CDC_ACM_DESC;

typedef struct
{
  /* Union Functional Descriptor*/
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bDescriptorSubtype;
  uint8_t bMasterInterface;
  uint8_t bSlaveInterface;
} __PACKED USBD_CDC_UNION_DESC;

#endif /* USBD_CDC_ACM_CLASS_ACTIVATED */


#if USBD_VIDEO_CLASS_ACTIVATED == 1U

/* Video Control Interface Descriptor (Interface Header) */
typedef struct
{
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bDescriptorSubtype;
  uint16_t bcdUVC;
  uint16_t wTotalLength;
  uint32_t dwClockFrequency;
  uint8_t bInCollection;
  uint8_t aInterfaceNr;
} __PACKED USBD_VIDEO_DESC;

/* Video Interface Descriptor (Input Terminal) */
typedef struct
{
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bDescriptorSubtype;
  uint8_t bTerminalID;
  uint16_t wTerminalType;
  uint8_t bAssocTerminal;
  uint8_t iTerminal;
#if (USBD_UVC_USE_CAMERA == 1U)
  uint16_t wObjectiveFocalLengthMin;
  uint16_t wObjectiveFocalLengthMax;
  uint16_t wOcularFocalLength;
  uint8_t bControlSize;
  uint8_t bmControls[3];
#endif /* USBD_UVC_USE_CAMERA */
} __PACKED USBD_VIDEO_INPUT_TERMINAL_DESC;

/* Video Interface Descriptor (Output Terminal) */
typedef struct
{
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bDescriptorSubtype;
  uint8_t bTerminalID;
  uint16_t wTerminalType;
  uint8_t bAssocTerminal;
  uint8_t bSourceID;
  uint8_t iTerminal;
} __PACKED USBD_VIDEO_OUTPUT_TERMINAL_DESC;

/* Video Streaming Interface Input Header Descriptor */
typedef struct
{
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bDescriptorSubtype;
  uint8_t bNumFormats;
  uint16_t wTotalLength;
  uint8_t bEndpointAddress;
  uint8_t bmInfo;
  uint8_t bTerminalLink;
  uint8_t bStillCaptureMethod;
  uint8_t bTriggerSupport;
  uint8_t bTriggerUsage;
  uint8_t bControlSize;
  uint8_t bmaControls;
} __PACKED USBD_VIDEO_VS_HEADER_DESC;

/* Video Format Descriptor */
typedef struct
{
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bDescriptorSubType;
  uint8_t bFormatIndex;
  uint8_t bNumFrameDescriptors;
#if (USBD_UVC_USE_H264 == 1U)
  uint8_t bDefaultFrameIndex;
  uint8_t bMaxCodecConfigDelay;
  uint8_t bmSupportedSliceModes;
  uint8_t bmSupportedSyncFrameTypes;
  uint8_t bResolutionScaling;
  uint8_t Reserved;
  uint8_t bmSupportedRateControlModes;
  uint16_t wMaxMBperSecOneResNoScal;
  uint16_t wMaxMBperSecTwoResNoScal;
  uint16_t wMaxMBperSecThreeResNoScal;
  uint16_t wMaxMBperSecFourResNoScal;
  uint16_t wMaxMBperSecOneResTemporalScal;
  uint16_t wMaxMBperSecTwoResTemporalScal;
  uint16_t wMaxMBperSecThreeResTemporalScal;
  uint16_t wMaxMBperSecFourResTemporalScal;
  uint16_t wMaxMBperSecOneResTemporalQualityScal;
  uint16_t wMaxMBperSecTwoResTemporalQualityScal;
  uint16_t wMaxMBperSecThreeResTemporalQualityScal;
  uint16_t wMaxMBperSecFourResTemporalQualityScal;
  uint16_t wMaxMBperSecOneResTemporalSpatialScal;
  uint16_t wMaxMBperSecTwoResTemporalSpatialScal;
  uint16_t wMaxMBperSecThreeResTemporalSpatialScal;
  uint16_t wMaxMBperSecFourResTemporalSpatialScal;
  uint16_t wMaxMBperSecOneResFullScal;
  uint16_t wMaxMBperSecTwoResFullScal;
  uint16_t wMaxMBperSecThreeResFullScal;
  uint16_t wMaxMBperSecFourResFullScal;
#elif (USBD_UVC_USE_FRAME_BASE_H264 == 1U)
  uint8_t pGuidFormat[16];
  uint8_t bBitsPerPixel;
  uint8_t bDefaultFrameIndex;
  uint8_t bAspectRatioX;
  uint8_t bAspectRatioY;
  uint8_t bmInterlaceFlag;
  uint8_t bCopyProtect;
  uint8_t bVariableSize;
#else
  uint8_t bmFlags;
  uint8_t bDefaultFrameIndex;
  uint8_t bAspectRatioX;
  uint8_t bAspectRatioY;
  uint8_t bmInterfaceFlag;
  uint8_t bCopyProtect;
#endif /* USBD_UVC_USE_H264 */
} __PACKED USBD_VIDEO_PAYLOAD_FORMAT_DESC;

/* Frame Descriptor */
typedef struct
{
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bDescriptorSubType;
  uint8_t bFrameIndex;
#if (USBD_UVC_USE_H264 == 1U)
  uint16_t wWidth;
  uint16_t wHeight;
  uint16_t wSARwidth;
  uint16_t wSARheight;
  uint16_t wProfile;
  uint8_t  bLevelIDC;
  uint16_t wConstrainedToolset;
  uint32_t bmSupportedUsages;
  uint16_t bmCapabilities;
  uint32_t bmSVCCapabilities;
  uint32_t bmMVCCapabilities;
  uint32_t dwMinBitRate;
  uint32_t dwMaxBitRate;
  uint32_t dwDefaultFrameInterval;
  uint8_t  bNumFrameIntervals;
  uint32_t dwFrameInterval;
#elif (USBD_UVC_USE_FRAME_BASE_H264 == 1U)
  uint8_t bmCapabilities;
  uint16_t wWidth;
  uint16_t wHeight;
  uint32_t dwMinBitRate;
  uint32_t dwMaxBitRate;
  uint32_t dwDefaultFrameInterval;
  uint8_t bFrameIntervalType;
  uint32_t dwBytesPerLine;
  uint32_t dwFrameInterval;
#else
  uint8_t bmCapabilities;
  uint16_t wWidth;
  uint16_t wHeight;
  uint32_t dwMinBitRate;
  uint32_t dwMaxBitRate;
  uint32_t dwMaxVideoFrameBufferSize;
  uint32_t dwDefaultFrameInterval;
  uint8_t bFrameIntervalType;
  uint32_t dwFrameInterval;
#endif /* USBD_UVC_USE_H264 */
} __PACKED USBD_VIDEO_FRAME_DESC;

typedef struct
{
  uint16_t bmHint;
  uint8_t bFormatIndex;
  uint8_t bFrameIndex;
  uint32_t dwFrameInterval;
  uint16_t wKeyFrameRate;
  uint16_t wPFrameRate;
  uint16_t wCompQuality;
  uint16_t wCompWindowSize;
  uint16_t wDelay;
  uint32_t dwMaxVideoFrameSize;
  uint32_t dwMaxPayloadTransferSize;
  uint32_t dwClockFrequency;
  uint8_t bmFramingInfo;
  uint8_t bPreferedVersion;
  uint8_t bMinVersion;
  uint8_t bMaxVersion;
#if (USBD_UVC_USE_FRAME_BASE_H264 == 1U)
  uint8_t bUsage;
  uint8_t bBitDepthLuma;
  uint8_t bmSettings;
  uint8_t bMaxNumberOfRefFramesPlus1;
  uint16_t bmRateControlModes;
  uint32_t bmLayoutPerStream;
#endif /* USBD_UVC_USE_FRAME_BASE_H264 */
} __PACKED USBD_VIDEO_CONTROL_DESC;

/* Processing Unit Descriptor */
typedef struct _USBD_VIDEO_PROCESSING_UNIT_DESCRIPTOR {
  uint8_t  bLength;
  uint8_t  bDescriptorType;
  uint8_t  bDescriptorSubtype;
  uint8_t  bUnitID;
  uint8_t  bSourceID;
  uint16_t wMaxMultiplier;
  uint8_t  bControlSize;
  uint8_t  bmControls[3];
  uint8_t  iProcessing;
  uint8_t  bmVideoStandards;
} __PACKED USBD_VIDEO_PROCESSING_UNIT_DESCRIPTOR;

#define VC_HEADER_SIZE             (sizeof(USBD_VIDEO_DESC) + \
                                    sizeof(USBD_VIDEO_INPUT_TERMINAL_DESC) + \
                                    sizeof(USBD_VIDEO_OUTPUT_TERMINAL_DESC))
#define VS_HEADER_SIZE             (sizeof(USBD_VIDEO_VS_HEADER_DESC) + \
                                    sizeof(USBD_VIDEO_PAYLOAD_FORMAT_DESC) + \
                                    sizeof(USBD_VIDEO_FRAME_DESC))

#endif /* USBD_VIDEO_CLASS_ACTIVATED */

/* Private macro -------------------------------------------------------------*/

/*------------------------------
  Device Descriptor - Device Qualifier Descriptor :
  ------------------------------*/

#ifndef USBD_VENDOR_ID
#define USBD_VENDOR_ID                          0x0483U
#endif

#ifndef USBD_PRODUCT_ID
#define USBD_PRODUCT_ID                         0x00U
#endif

#ifndef USBD_BCDUSB
#define USBD_BCDUSB                             0x0200U
#endif

#define USBD_CLASS                              0x00U
#define USBD_SUBCLASS                           0x00U
#define USBD_PROTOCOL                           0x00U

#ifndef USBD_MAX_PACKET_SIZE_EP0
#define USBD_MAX_PACKET_SIZE_EP0                0x40U
#endif

#ifndef USBD_BCD_DEVICE
#define USBD_BCD_DEVICE                         0x0200U
#endif

#ifndef USBD_IDX_MANUFACTURE_STR
#define USBD_IDX_MANUFACTURE_STR                0x01U
#endif

#ifndef USBD_IDX_PRODUCT_STR
#define USBD_IDX_PRODUCT_STR                    0x02U
#endif

#ifndef USBD_IDX_SERIAL_STR
#define USBD_IDX_SERIAL_STR                     0x03U
#endif

#ifndef USBD_MAX_NUM_CONFIGURATION
#define USBD_MAX_NUM_CONFIGURATION              0x01U
#endif

#ifndef USBD_RESERVED
#define USBD_RESERVED                           0x00U
#endif

#define USBD_COMPOSITE_CLASS                    0xEFU
#define USBD_COMPOSITE_SUBCLASS                 0x02U
#define USBD_COMPOSITE_PROTOCOL                 0x01U

#if USBD_CDC_ACM_CLASS_ACTIVATED == 1U
#define USBD_CDCACM_CLASS                       0x02U
#define USBD_CDCACM_SUBCLASS                    0x02U
#define USBD_CDCACM_PROTOCOL                    0x00U
#endif /* USBD_CDC_ACM_CLASS_ACTIVATED */

/*------------------------------
  Configuration Descriptor:
  ------------------------------*/

#define USBD_CONFIG_TOTAL_LENGTH                0x00U

#ifndef USBD_NUM_INTERFACES
#define USBD_NUM_INTERFACES                     0x00U
#endif

#ifndef USBD_CONFIG_VALUE
#define USBD_CONFIG_VALUE                       0x01U
#endif

#ifndef USBD_CONFIG_STR_DESC_IDX
#define USBD_CONFIG_STR_DESC_IDX                0x00U
#endif

#ifndef USBD_CONFIG_BMATTRIBUTES
#define USBD_CONFIG_BMATTRIBUTES                0xC0U
#endif

#ifndef USBD_CONFIG_MAXPOWER
#define USBD_CONFIG_MAXPOWER                    0x19U
#endif

#ifndef USBD_MANUFACTURER_STRING
#define USBD_MANUFACTURER_STRING                "STMicroelectronics"
#endif

#ifndef USBD_PRODUCT_STRING
#define USBD_PRODUCT_STRING                     "STM32 USB Device"
#endif

#ifndef USBD_SERIAL_NUMBER_STRING
#define USBD_SERIAL_NUMBER_STRING               "STM32_DEVICE001"
#endif

#ifndef USBD_LANG_ID_STRING
#define USBD_LANG_ID_STRING                     0x0409
#endif

#if USBD_HID_CLASS_ACTIVATED == 1U
/*------------------------------
  HID HID Descriptor:
  ------------------------------*/

#ifndef USBD_HID_BCD
#define USBD_HID_BCD                            0x0111U
#endif

#ifndef USBD_HID_BCOUNTRY_CODE
#define USBD_HID_BCOUNTRY_CODE                  0x00U
#endif

#ifndef USBD_HID_NUM_DESCRIPTORS
#define USBD_HID_NUM_DESCRIPTORS                0x01U
#endif

#ifndef USBD_HID_DESCRIPTOR_TYPE
#define USBD_HID_DESCRIPTOR_TYPE                0x22U
#endif

#if USBD_HID_MOUSE_ACTIVATED == 1U
/*------------------------------
  HID Mouse Interface Descriptor:
  ------------------------------*/

#define USBD_HID_MOUSE_ITF_NUMBERS              0x00U
#define USBD_HID_MOUSE_ITF_ATL_SETTING          0x00U
#define USBD_HID_MOUSE_ITF_EP_NUMBERS           0x01U
#define USBD_HID_MOUSE_ITF_CLASS                UX_DEVICE_CLASS_HID_CLASS
#define USBD_HID_MOUSE_ITF_PROTOCOL             USBD_HID_MOUSE_INTERFACE

#ifndef USBD_HID_MOUSE_ITF_SUBCLASS
#define USBD_HID_MOUSE_ITF_SUBCLASS             0x01U
#endif /* USBD_HID_MOUSE_ITF_SUBCLASS */

#ifndef USBD_HID_MOUSE_ITF_STR_DESC_IDX
#define USBD_HID_MOUSE_ITF_STR_DESC_IDX         0x00U
#endif /* USBD_HID_MOUSE_ITF_STR_DESC_IDX */

#define USBD_HID_MOUSE_EPIN_TYPE                UX_INTERRUPT_ENDPOINT

#endif /* USBD_HID_MOUSE_ACTIVATED */

#if USBD_HID_KEYBOARD_ACTIVATED == 1U
/*------------------------------
  HID Keyboard Interface Descriptor:
  ------------------------------*/

#define USBD_HID_KEYBOARD_ITF_NUMBERS           0x00U
#define USBD_HID_KEYBOARD_ITF_ATL_SETTING       0x00U
#define USBD_HID_KEYBOARD_ITF_EP_NUMBERS        0x01U
#define USBD_HID_KEYBOARD_ITF_CLASS             UX_DEVICE_CLASS_HID_CLASS
#define USBD_HID_KEYBOARD_ITF_PROTOCOL          USBD_HID_KEYBOARD_INTERFACE

#ifndef USBD_HID_KEYBOARD_ITF_SUBCLASS
#define USBD_HID_KEYBOARD_ITF_SUBCLASS          0x01U
#endif /* USBD_HID_KEYBOARD_ITF_SUBCLASS */

#ifndef USBD_HID_KEYBOARD_ITF_STR_DESC_IDX
#define USBD_HID_KEYBOARD_ITF_STR_DESC_IDX      0x00U
#endif /* USBD_HID_KEYBOARD_ITF_STR_DESC_IDX */

#define USBD_HID_KEYBOARD_EPIN_TYPE             UX_INTERRUPT_ENDPOINT

#endif /* USBD_HID_KEYBOARD_ACTIVATED */

#if USBD_HID_CUSTOM_ACTIVATED == 1U
/*------------------------------
  HID custom Interface Descriptor:
  ------------------------------*/

#define USBD_HID_CUSTOM_ITF_NUMBERS              0x00U
#define USBD_HID_CUSTOM_ITF_ATL_SETTING          0x00U
#define USBD_HID_CUSTOM_ITF_EP_NUMBERS           0x02U
#define USBD_HID_CUSTOM_ITF_CLASS                UX_DEVICE_CLASS_HID_CLASS
#define USBD_HID_CUSTOM_ITF_PROTOCOL             USBD_HID_CUSTOM_INTERFACE

#ifndef USBD_HID_CUSTOM_ITF_SUBCLASS
#define USBD_HID_CUSTOM_ITF_SUBCLASS             0x00U
#endif /* USBD_HID_CUSTOM_ITF_SUBCLASS */

#ifndef USBD_HID_CUSTOM_ITF_STR_DESC_IDX
#define USBD_HID_CUSTOM_ITF_STR_DESC_IDX         0x00U
#endif /* USBD_HID_CUSTOM_ITF_STR_DESC_IDX */

#define USBD_HID_CUSTOM_EPIN_TYPE                UX_INTERRUPT_ENDPOINT
#define USBD_HID_CUSTOM_EPOUT_TYPE               UX_INTERRUPT_ENDPOINT

#endif /* USBD_HID_CUSTOM_ACTIVATED */
#endif /* USBD_HID_CLASS_ACTIVATED */


#if USBD_CDC_ACM_CLASS_ACTIVATED == 1U
/*------------------------------
  CDC ACM Interface Descriptor:
  ------------------------------*/
#ifndef USBD_CDC_BCD
#define USBD_CDC_BCD                            0x0110
#endif

#ifndef USBD_CDC_BCOUNTRY_CODE
#define USBD_CDC_BCOUNTRY_CODE                  0x00U
#endif /* USBD_CDC_BCOUNTRY_CODE */

#ifndef USBD_CDC_NUM_DESCRIPTORS
#define USBD_CDC_NUM_DESCRIPTORS                0x01U
#endif /* USBD_CDC_NUM_DESCRIPTORS */

#ifndef USBD_CDC_DESCRIPTOR_TYPE
#define USBD_CDC_DESCRIPTOR_TYPE                0x24U
#endif /* USBD_CDC_DESCRIPTOR_TYPE */

/*------------------------------
  CDC ACM Interface Descriptor:
  ------------------------------*/

/* Interface control for CDC_ACM */
#define USBD_CDC_ACM_CTL_NUMBERS                1
#define USBD_CDC_ACM_CTL_ATL_SETTING            0
#define USBD_CDC_ACM_CTL_EP_NUMBERS             1
#define USBD_CDC_ACM_CTL_CLASS                  0x02
#define USBD_CDC_ACM_CTL_SUBCLASS               0x02
#define USBD_CDC_ACM_CTL_PROTOCOL               0x01
#define USBD_CDC_ACM_CTL_STR_DESC_IDX           0

/* Interface data for CDC_ACM */
#define USBD_CDC_ACM_DATA_NUMBERS               2
#define USBD_CDC_ACM_DATA_ATL_SETTING           0
#define USBD_CDC_ACM_DATA_EP_NUMBERS            2
#define USBD_CDC_ACM_DATA_CLASS                 0x0A
#define USBD_CDC_ACM_DATA_SUBCLASS              0x00
#define USBD_CDC_ACM_DATA_PROTOCOL              0x00
#define USBD_CDC_ACM_DATA_STR_DESC_IDX          0

#endif /* USBD_CDC_ACM_CLASS_ACTIVATED */

#if USBD_MSC_CLASS_ACTIVATED == 1U
/*------------------------------
  MSC Interface Descriptor:
  ------------------------------*/

#define USBD_MSC_ITF_NUMBERS                    0x01U
#define USBD_MSC_ITF_ATL_SETTING                0x00U
#define USBD_MSC_ITF_EP_NUMBERS                 0x02U
#define USBD_MSC_ITF_CLASS                      0x08U
#define USBD_MSC_ITF_PROTOCOL                   0x50U

#ifndef USBD_MSC_ITF_SUBCLASS
#define USBD_MSC_ITF_SUBCLASS                   0x06U
#endif /* USBD_MSC_ITF_SUBCLASS */

#ifndef USBD_MSC_ITF_STR_DESC_IDX
#define USBD_MSC_ITF_STR_DESC_IDX               0x00U
#endif /* USBD_MSC_ITF_STR_DESC_IDX */

#endif /* USBD_MSC_CLASS_ACTIVATED */

#if USBD_DFU_CLASS_ACTIVATED == 1U
/*------------------------------
  DFU Interface Descriptor:
  ------------------------------*/

#define USBD_DFU_ITF_NUMBERS                    0x00U
#define USBD_DFU_ITF_ATL_SETTING                0x00U
#define USBD_DFU_ITF_EP_NUMBERS                 0x00U
#define USBD_DFU_ITF_CLASS                      0xFEU
#define USBD_DFU_ITF_PROTOCOL                   0x02U

#ifndef USBD_DFU_ITF_SUBCLASS
#define USBD_DFU_ITF_SUBCLASS                   0x01U
#endif /* USBD_DFU_ITF_SUBCLASS */

#ifndef USBD_DFU_ITF_STR_DESC_IDX
#ifdef USBD_DFU_STRING_DESC_INDEX
#define USBD_DFU_ITF_STR_DESC_IDX               USBD_DFU_STRING_DESC_INDEX
#else
#define USBD_DFU_ITF_STR_DESC_IDX               0x00U
#endif
#endif /* USBD_DFU_ITF_STR_DESC_IDX */

#ifndef USBD_DFU_BCD_VERSION
#define USBD_DFU_BCD_VERSION                    0x011AU
#endif

#ifndef USBD_DFU_DESCRIPTOR_TYPE
#define USBD_DFU_DESCRIPTOR_TYPE                0x21U
#endif

#ifndef USBD_DFU_BM_ATTRIBUTES
#define USBD_DFU_BM_ATTRIBUTES                  0x0BU
#endif

#ifndef USBD_DFU_DETACH_TIMEOUT
#define USBD_DFU_DETACH_TIMEOUT                 255U
#endif

#ifndef USBD_DFU_XFER_SIZE
#define USBD_DFU_XFER_SIZE                      1024U
#endif

#endif /* USBD_DFU_CLASS_ACTIVATED */

#if USBD_AUDIO_CLASS_ACTIVATED == 1U
/*------------------------------
  AUDIO Interface Descriptor:
  ------------------------------*/

#define USBD_AUDIO_CONTROL_ITF_NUMBERS          0x00U
#define USBD_AUDIO_CONTROL_ITF_ATL_SETTING      0x00U
#ifdef USBD_AUDIO_EPINTERRUPT_SUPPORTED
#define USBD_AUDIO_CONTROL_ITF_EP_NUMBERS       0x01U
#else
#define USBD_AUDIO_CONTROL_ITF_EP_NUMBERS       0x00U
#endif
#define USBD_AUDIO_CONTROL_ITF_CLASS            0x01U
#define USBD_AUDIO_CONTROL_ITF_SUBCLASS         0x01U
#define USBD_AUDIO_CONTROL_ITF_PROTOCOL         0x20U

#define USBD_AUDIO_STREAM_IN_ITF_NUMBERS        0x01U
#define USBD_AUDIO_STREAM_IN_ITF_ATL_SETTING    0x00U
#define USBD_AUDIO_STREAM_IN_ITF_EP_NUMBERS     0x00U
#define USBD_AUDIO_STREAM_IN_ITF_CLASS          0x01U
#define USBD_AUDIO_STREAM_IN_ITF_SUBCLASS       0x02U
#define USBD_AUDIO_STREAM_IN_ITF_PROTOCOL       0x20U

#define USBD_AUDIO_STREAM_OUT_ITF_NUMBERS       0x02U
#define USBD_AUDIO_STREAM_OUT_ITF_ATL_SETTING   0x00U
#define USBD_AUDIO_STREAM_OUT_ITF_EP_NUMBERS    0x00U
#define USBD_AUDIO_STREAM_OUT_ITF_CLASS         0x01U
#define USBD_AUDIO_STREAM_OUT_ITF_SUBCLASS      0x02U
#define USBD_AUDIO_STREAM_OUT_ITF_PROTOCOL      0x20U

#define USBD_AUDIO_STREAM_ALT_ITF_ATL_SETTING   0x01U
#define USBD_AUDIO_STREAM_IN_ALT_EP_NUMBERS     0x01U

#ifdef USBD_AUDIO_EPFEEDBACK_SUPPORTED
#define USBD_AUDIO_STREAM_OUT_ALT_EP_NUMBERS    0x02U
#else
#define USBD_AUDIO_STREAM_OUT_ALT_EP_NUMBERS    0x01U
#endif

#ifndef USBD_AUDIO_ITF_STR_DESC_IDX
#define USBD_AUDIO_ITF_STR_DESC_IDX             0x00U
#endif /* USBD_AUDIO_ITF_STR_DESC_IDX */

#ifndef USBD_AUDIO_CATEGORY
#define USBD_AUDIO_CATEGORY                     0x08U
#endif

#ifndef USBD_AUDIO_FU_CONTROL_MUTE
#define USBD_AUDIO_FU_CONTROL_MUTE              0x0003U
#endif

#ifndef USBD_AUDIO_FU_CONTROL_VOLUME
#define USBD_AUDIO_FU_CONTROL_VOLUME            0x000CU
#endif

#ifndef USBD_AUDIO_CAPTURE_TERMINAL_INPUT_ID
#define USBD_AUDIO_CAPTURE_TERMINAL_INPUT_ID    0x01U
#endif

#ifndef USBD_AUDIO_CAPTURE_FEATURE_UNIT_ID
#define USBD_AUDIO_CAPTURE_FEATURE_UNIT_ID      0x02U
#endif

#ifndef USBD_AUDIO_CAPTURE_TERMINAL_OUTPUT_ID
#define USBD_AUDIO_CAPTURE_TERMINAL_OUTPUT_ID   0x03U
#endif

#ifndef USBD_AUDIO_PLAY_TERMINAL_INPUT_ID
#define USBD_AUDIO_PLAY_TERMINAL_INPUT_ID       0x04U
#endif

#ifndef USBD_AUDIO_PLAY_FEATURE_UNIT_ID
#define USBD_AUDIO_PLAY_FEATURE_UNIT_ID         0x05U
#endif

#ifndef USBD_AUDIO_PLAY_TERMINAL_OUTPUT_ID
#define USBD_AUDIO_PLAY_TERMINAL_OUTPUT_ID      0x06U
#endif

#ifndef USBD_AUDIO_CLOCK_SOURCE_ID
#define USBD_AUDIO_CLOCK_SOURCE_ID              0x10U
#endif

#ifndef USBD_AUDIO_CHANNEL_COUNT
#define USBD_AUDIO_CHANNEL_COUNT                0x02U
#endif

#ifndef USBD_AUDIO_CHANNEL_MAP
#define USBD_AUDIO_CHANNEL_MAP                  0x00000003UL
#endif

#ifndef USBD_AUDIO_RES_BYTE
#define USBD_AUDIO_RES_BYTE                     0x02U
#endif

#ifndef USBD_AUDIO_RES_BIT
#define USBD_AUDIO_RES_BIT                      0x10U
#endif

#define USBD_AUDIO_DATA_EP_TYPE                 0x05U
#define USBD_AUDIO_FEEDBACK_EP_TYPE             0x11U

#define USBD_AUDIO_CONTROL_INTERFACE_SIZE       (sizeof(USBD_AUDIO_AC_CS_IF_DESC) + \
                                                 sizeof(USBD_AUDIO_CLOCK_SOURCE_DESC) + \
                                                 sizeof(USBD_AUDIO_INPUT_TERMINAL_DESC) + \
                                                 sizeof(USBD_AUDIO_FEATURE_UNIT_DESC) + \
                                                 sizeof(USBD_AUDIO_OUTPUT_TERMINAL_DESC) + \
                                                 sizeof(USBD_AUDIO_INPUT_TERMINAL_DESC) + \
                                                 sizeof(USBD_AUDIO_FEATURE_UNIT_DESC) + \
                                                 sizeof(USBD_AUDIO_OUTPUT_TERMINAL_DESC))

#endif /* USBD_AUDIO_CLASS_ACTIVATED */

#if USBD_MTP_CLASS_ACTIVATED == 1U

/*------------------------------
  MTP Interface Descriptor:
  ------------------------------*/

#define USBD_MTP_ITF_NUMBERS                    1
#define USBD_MTP_ITF_ATL_SETTING                0
#define USBD_MTP_ITF_EP_NUMBERS                 3
#define USBD_MTP_ITF_CLASS                      0x06
#define USBD_MTP_ITF_PROTOCOL                   0x01

#ifndef USBD_MTP_ITF_SUBCLASS
#define USBD_MTP_ITF_SUBCLASS                   0x01
#endif

#ifndef USBD_MTP_ITF_STR_DESC_IDX
#define USBD_MTP_ITF_STR_DESC_IDX               0
#endif

#endif /* USBD_MTP_CLASS_ACTIVATED */

#if USBD_VIDEO_CLASS_ACTIVATED == 1U
/*------------------------------
  VIDEO Interface Descriptor:
  ------------------------------*/
#ifndef UVC_FRAME_WIDTH
#define UVC_FRAME_WIDTH                               320U
#endif

#ifndef UVC_FRAME_HEIGHT
#define UVC_FRAME_HEIGHT                              236U
#endif

#ifndef UVC_CAM_FPS_FS
#define UVC_CAM_FPS_FS                                10U
#endif

#ifndef UVC_CAM_FPS_HS
#define UVC_CAM_FPS_HS                                5U
#endif

#define UVC_MIN_BIT_RATE(n)                           (UVC_FRAME_WIDTH * UVC_FRAME_HEIGHT * 16U * (n))
#define UVC_MAX_BIT_RATE(n)                           (UVC_FRAME_WIDTH * UVC_FRAME_HEIGHT * 16U * (n))
#define UVC_INTERVAL(n)                               (10000000U/(n))

#define UVC_MAX_FRAME_SIZE                            (UVC_FRAME_WIDTH * UVC_FRAME_HEIGHT * 16U / 2U)

/*------------------------------
  VIDEO Interface Descriptor:
  ------------------------------*/

/* VIDEO Interface */
#define USBD_VIDEO_ITF_NUMBERS               1
#define USBD_VIDEO_ITF_ATL_SETTING           0
#define USBD_VIDEO_ITF_EP_NUMBERS            0
#define USBD_VIDEO_ITF_CLASS                 0x0E
#define USBD_VIDEO_ITF_PROTOCOL              0
#define USBD_VIDEO_ITF_SUBCLASS              1
#define USBD_VIDEO_ITF_STR_DESC_IDX          0

/* Standard VS (Video Streaming) Interface */
/* Interface 1, Alternate Setting 0 */
#define USBD_VIDEO_VS_ITF_NUMBERS            1
#define USBD_VIDEO_VS_ITF_ATL_SETTING        0
#define USBD_VIDEO_VS_ITF_EP_NUMBERS         0
#define USBD_VIDEO_VS_ITF_CLASS              0x0E
#define USBD_VIDEO_VS_ITF_PROTOCOL           0
#define USBD_VIDEO_VS_ITF_SUBCLASS           2
#define USBD_VIDEO_VS_ITF_STR_DESC_IDX       0

/* Standard VS (Video Streaming) Interface */
/* Interface 1, Alternate Setting 1 */
#define USBD_VIDEO_VS_AS_ITF_NUMBERS            1
#define USBD_VIDEO_VS_AS_ITF_ATL_SETTING        1
#define USBD_VIDEO_VS_AS_ITF_EP_NUMBERS         1
#define USBD_VIDEO_VS_AS_ITF_CLASS              0x0E
#define USBD_VIDEO_VS_AS_ITF_PROTOCOL           0
#define USBD_VIDEO_VS_AS_ITF_SUBCLASS           2
#define USBD_VIDEO_VS_AS_ITF_STR_DESC_IDX       0

#endif /* USBD_VIDEO_CLASS_ACTIVATED */

#if USBD_PRINTER_CLASS_ACTIVATED == 1U
/*------------------------------
  PRINTER Interface Descriptor:
  ------------------------------*/

#define USBD_PRINTER_ITF_NUMBERS                0x01U
#define USBD_PRINTER_ITF_ATL_SETTING            0x00U
#define USBD_PRINTER_ITF_EP_NUMBERS             0x02U
#define USBD_PRINTER_ITF_CLASS                  0x07U
#define USBD_PRINTER_ITF_PROTOCOL               0x02U

#ifndef USBD_PRINTER_ITF_SUBCLASS
#define USBD_PRINTER_ITF_SUBCLASS               0x01U
#endif /* USBD_PRINTER_ITF_SUBCLASS */

#ifndef USBD_PRINTER_ITF_STR_DESC_IDX
#define USBD_PRINTER_ITF_STR_DESC_IDX           0x00U
#endif /* USBD_PRINTER_ITF_STR_DESC_IDX */

#endif /* USBD_PRINTER_CLASS_ACTIVATED */

#if USBD_CCID_CLASS_ACTIVATED == 1U
/*------------------------------
  CCID Interface Descriptor:
  ------------------------------*/

#define USBD_CCID_ITF_NUMBERS                   0x00U
#define USBD_CCID_ITF_ATL_SETTING               0x00U
#define USBD_CCID_ITF_EP_NUMBERS                0x03U
#define USBD_CCID_ITF_CLASS                     UX_DEVICE_CLASS_CCID_CLASS
#define USBD_CCID_ITF_SUBCLASS                  UX_DEVICE_CLASS_CCID_SUBCLASS
#define USBD_CCID_ITF_PROTOCOL                  UX_DEVICE_CLASS_CCID_PROTOCOL

#ifndef USBD_CCID_ITF_STR_DESC_IDX
#define USBD_CCID_ITF_STR_DESC_IDX              0x00U
#endif /* USBD_CCID_ITF_STR_DESC_IDX */

/*
 * Override these defaults to match the attached CCID smartcard interface.
 * The provided values describe a minimal single-slot reader profile.
 */
#ifndef USBD_CCID_MAX_SLOT_INDEX
#define USBD_CCID_MAX_SLOT_INDEX                0x00U
#endif

#ifndef USBD_CCID_VOLTAGE_SUPPLY
#define USBD_CCID_VOLTAGE_SUPPLY                0x07U
#endif

#ifndef USBD_CCID_DEFAULT_CLOCK_FREQ
#define USBD_CCID_DEFAULT_CLOCK_FREQ            4000UL
#endif

#ifndef USBD_CCID_MAX_CLOCK_FREQ
#define USBD_CCID_MAX_CLOCK_FREQ                4000UL
#endif

#ifndef USBD_CCID_N_CLOCKS
#define USBD_CCID_N_CLOCKS                      0x00U
#endif

#ifndef USBD_CCID_DEFAULT_DATA_RATE
#define USBD_CCID_DEFAULT_DATA_RATE             9600UL
#endif

#ifndef USBD_CCID_MAX_DATA_RATE
#define USBD_CCID_MAX_DATA_RATE                 9600UL
#endif

#ifndef USBD_CCID_N_DATA_RATES
#define USBD_CCID_N_DATA_RATES                  0x00U
#endif

#ifndef USBD_CCID_PROTOCOL
#define USBD_CCID_PROTOCOL                     0x00000003UL
#endif

#ifndef USBD_CCID_CLASS_GET_RESPONSE
#define USBD_CCID_CLASS_GET_RESPONSE            0xFFU
#endif

#ifndef USBD_CCID_CLASS_ENVELOPE
#define USBD_CCID_CLASS_ENVELOPE                0xFFU
#endif

#ifndef USBD_CCID_LCD_LAYOUT
#define USBD_CCID_LCD_LAYOUT                    0x0000U
#endif

#ifndef USBD_CCID_PIN_SUPPORT
#define USBD_CCID_PIN_SUPPORT                   0U
#endif

#ifndef USBD_CCID_FEATURES
#define USBD_CCID_FEATURES                      0x000407B8UL
#endif

#ifndef USBD_CCID_MAX_MESSAGE_LENGTH
#define USBD_CCID_MAX_MESSAGE_LENGTH            512U
#endif

#ifndef USBD_CCID_MAX_IFSD
#define USBD_CCID_MAX_IFSD                      0U
#endif

#ifndef USBD_CCID_SYNCH_PROTOCOLS
#define USBD_CCID_SYNCH_PROTOCOLS               0x00000007UL
#endif

#ifndef USBD_CCID_MECHANICAL
#define USBD_CCID_MECHANICAL                    0U
#endif

#ifndef USBD_CCID_MAX_BUSY_SLOTS
#define USBD_CCID_MAX_BUSY_SLOTS                0x01U
#endif

#endif /* USBD_CCID_CLASS_ACTIVATED */

/*------------------------------
  USB DEVICE FRAMEWORK Variable:
  ------------------------------*/
#ifndef USBD_FRAMEWORK_FS_MAX_LENGTH
#define USBD_FRAMEWORK_FS_MAX_LENGTH            0x100U
#endif

#ifndef USBD_FRAMEWORK_HS_MAX_LENGTH
#define USBD_FRAMEWORK_HS_MAX_LENGTH            0x100U
#endif

#ifndef USBD_STRING_FRAMEWORK_MAX_LENGTH
#define USBD_STRING_FRAMEWORK_MAX_LENGTH        0x100U
#endif

#ifndef USBD_LANGUAGE_ID_MAX_LENGTH
#define USBD_LANGUAGE_ID_MAX_LENGTH             0x02U
#endif

#if USBD_HID_MOUSE_ACTIVATED == 1U
extern USB_DEVICE_INTERFACE_HANDLE hid_mouse_interface[];
#endif /* USBD_HID_MOUSE_ACTIVATED */

#if USBD_HID_KEYBOARD_ACTIVATED == 1U
extern USB_DEVICE_INTERFACE_HANDLE hid_keyboard_interface[];
#endif /* USBD_HID_KEYBOARD_ACTIVATED */

#if USBD_HID_CUSTOM_ACTIVATED == 1U
extern USB_DEVICE_INTERFACE_HANDLE hid_custom_interface[];
#endif /* USBD_HID_CUSTOM_ACTIVATED */

#if USBD_CDC_ACM_CLASS_ACTIVATED == 1U
extern USB_DEVICE_INTERFACE_HANDLE cdc_acm_interface[];
#endif /* USBD_CDC_ACM_CLASS_ACTIVATED */

#if USBD_MSC_CLASS_ACTIVATED == 1U
extern USB_DEVICE_INTERFACE_HANDLE msc_interface[];
#endif /* USBD_MSC_CLASS_ACTIVATED */

#if USBD_DFU_CLASS_ACTIVATED == 1U
extern USB_DEVICE_INTERFACE_HANDLE dfu_interface[];
#endif /* USBD_DFU_CLASS_ACTIVATED */

#if USBD_AUDIO_CLASS_ACTIVATED == 1U
extern USB_DEVICE_INTERFACE_HANDLE audio_interface[];
#endif /* USBD_AUDIO_CLASS_ACTIVATED */

#if USBD_VIDEO_CLASS_ACTIVATED == 1U
extern USB_DEVICE_INTERFACE_HANDLE video_interface[];
#endif /* USBD_VIDEO_CLASS_ACTIVATED */

#if USBD_PRINTER_CLASS_ACTIVATED == 1U
extern USB_DEVICE_INTERFACE_HANDLE printer_interface[];
#endif /* USBD_PRINTER_CLASS_ACTIVATED */

#if USBD_CCID_CLASS_ACTIVATED == 1U
extern USB_DEVICE_INTERFACE_HANDLE ccid_interface[];
#endif /* USBD_CCID_CLASS_ACTIVATED */

#if USBD_MTP_CLASS_ACTIVATED == 1U
extern USB_DEVICE_INTERFACE_HANDLE mtp_interface[];
#endif /* USBD_MTP_CLASS_ACTIVATED */

#define USBD_DESCRIPTOR_SUCCESS                 0x00U
#define USBD_DESCRIPTOR_ERROR                   0xFFU

#define USBD_DESCRIPTOR_MEMORY_INSUFFICIENT     0x11U
#define USBD_DESCRIPTOR_CLASS_FULL              0x12U
#define USBD_DESCRIPTOR_CLASS_BUILDER_ERROR     0x13U


/* Exported functions prototypes ---------------------------------------------*/
UINT usb_device_descriptor_register_class(USB_DEVICE_ClASS_TYPE class_type,
                                          USB_DEVICE_INTERFACE_HANDLE *class_interfaces,
                                          ULONG *parameter);

UINT usb_device_add_string_descriptor(USBD_USER_STRING_DESC *user_string_desc);

UINT usb_device_descriptor_get_framework(USB_DESCRIPTOR *usbd_descriptor);

UINT usb_device_framework_set_interface(ULONG p_config_framework,
                                        ULONG *config_framework_size,
                                        USBD_INTERFACE_DESC *p_interface_desc,
                                        USB_DEVICE_INTERFACE_HANDLE *p_interface);

UINT usb_device_framework_set_endpoint(ULONG p_config_framework,
                                       ULONG *config_framework_size,
                                       USBD_ENDPOINT_DESC *p_endpoint_desc,
                                       USB_DEVICE_ENDPOINT_HANDLE *p_endpoint,
                                       UCHAR device_speed);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* UX_STM32_DEVICE_DESCRIPTORS_H */

