/**
********************************************************************************
* file           : ux_stm32_device_descriptors.c
* brief          : USBX Device descriptor header file
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

/* Includes ------------------------------------------------------------------*/
#include "ux_stm32_device_descriptors.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USB_DEVICE_HANDLE               device_handle;
USBD_USER_STRING_DESC_HANDLE    string_descriptor_handle;

#if USBD_HIGH_SPEED_SUPPORTED == 1U
static __ALIGNED(4) UCHAR usb_device_framework_high_speed[USBD_FRAMEWORK_HS_MAX_LENGTH] = {0};
#endif /* USBD_HIGH_SPEED_SUPPORTED */

static __ALIGNED(4) UCHAR usb_device_framework_full_speed[USBD_FRAMEWORK_FS_MAX_LENGTH] = {0};
static __ALIGNED(4) UCHAR usb_device_string_framework[USBD_STRING_FRAMEWORK_MAX_LENGTH] = {0};
static __ALIGNED(4) UCHAR usb_device_language_id_framework[USBD_LANGUAGE_ID_MAX_LENGTH] = {0};

/* Private functions prototypes ----------------------------------------------*/
static UINT usb_device_framework_builder(USB_DEVICE_HANDLE *p_device,
                                         UCHAR *framework,
                                         ULONG *framework_length,
                                         UCHAR device_speed);

static UINT usb_device_string_framework_builder(USBD_USER_STRING_DESC_HANDLE *p_string_descriptor,
                                                UCHAR* framework_pointer,
                                                ULONG *framework_length);

static UINT usb_device_language_id_framework_builder(UCHAR* framework_pointer,
                                                     ULONG *framework_length);

#if USBD_MSC_CLASS_ACTIVATED == 1U
static UINT usb_device_msc_framework_builder(USB_DEVICE_INTERFACE_HANDLE *p_interface,
                                             ULONG p_config_framework,
                                             ULONG *config_framework_size,
                                             UCHAR device_speed);
#endif /* USBD_MSC_CLASS_ACTIVATED */

#if USBD_DFU_CLASS_ACTIVATED == 1U
static UINT usb_device_dfu_framework_builder(USB_DEVICE_INTERFACE_HANDLE *p_interface,
                                             ULONG p_config_framework,
                                             ULONG *config_framework_size,
                                             UCHAR device_speed);
#endif /* USBD_DFU_CLASS_ACTIVATED */

#if USBD_AUDIO_CLASS_ACTIVATED == 1U
static UINT usb_device_audio_framework_builder(USB_DEVICE_INTERFACE_HANDLE *p_interface,
                                               ULONG p_config_framework,
                                               ULONG *config_framework_size,
                                               UCHAR device_speed);
#endif /* USBD_AUDIO_CLASS_ACTIVATED */

#if USBD_PRINTER_CLASS_ACTIVATED == 1U
static UINT usb_device_printer_framework_builder(USB_DEVICE_INTERFACE_HANDLE *p_interface,
                                                 ULONG p_config_framework,
                                                 ULONG *config_framework_size,
                                                 UCHAR device_speed);
#endif /* USBD_PRINTER_CLASS_ACTIVATED */

#if USBD_CCID_CLASS_ACTIVATED == 1U
static UINT usb_device_ccid_framework_builder(USB_DEVICE_INTERFACE_HANDLE *p_interface,
                                              ULONG p_config_framework,
                                              ULONG *config_framework_size,
                                              UCHAR device_speed);
#endif /* USBD_CCID_CLASS_ACTIVATED */


#if USBD_HID_CLASS_ACTIVATED == 1U
static UINT usb_device_hid_framework_builder(USB_DEVICE_INTERFACE_HANDLE *p_interface,
                                             ULONG p_config_framework,
                                             ULONG *config_framework_size,
                                             UCHAR device_speed);
#endif /* USBD_HID_CLASS_ACTIVATED */

#if USBD_CDC_ACM_CLASS_ACTIVATED == 1U
static UINT usb_device_cdc_acm_framework_builder(USB_DEVICE_INTERFACE_HANDLE *p_interface,
                                                 ULONG p_config_framework,
                                                 ULONG *config_framework_size,
                                                 UCHAR device_speed);
#endif /* USBD_CDC_ACM_CLASS_ACTIVATED */

#if USBD_MTP_CLASS_ACTIVATED == 1U
static UINT usb_device_mtp_framework_builder(USB_DEVICE_INTERFACE_HANDLE *p_interface,
                                             ULONG p_config_framework,
                                             ULONG *config_framework_size,
                                             UCHAR device_speed);
#endif /* USBD_MTP_CLASS_ACTIVATED */

#if USBD_VIDEO_CLASS_ACTIVATED == 1U
#if (USBD_UVC_USE_FRAME_BASE_H264 == 1U)
static uint8_t usbd_uvc_guid[16] = {0x48, 0x32, 0x36, 0x34,
                                    0x00, 0x00, 0x10, 0x00,
                                    0x80, 0x00, 0x00, 0xAA,
                                    0x00, 0x38, 0x9B, 0x71};
#endif /* USBD_UVC_USE_FRAME_BASE_H264 */

static UINT usb_device_video_framework_builder(USB_DEVICE_INTERFACE_HANDLE *p_interface,
                                               ULONG p_config_framework,
                                               ULONG *config_framework_size,
                                               UCHAR device_speed);
#endif /* USBD_VIDEO_CLASS_ACTIVATED */
/**
  * @brief  usb_device_descriptor_register_class
  *         USB device register class
  * @retval status
  */
UINT usb_device_descriptor_register_class(USB_DEVICE_ClASS_TYPE class_type,
                                          USB_DEVICE_INTERFACE_HANDLE *class_interfaces,
                                          ULONG *parameter)
{
  UCHAR class_index = 0U;
  UCHAR itf_index = 0U;
  USB_DEVICE_INTERFACE_HANDLE *p_interface;

  if ((class_type == USBD_CLASS_TYPE_NONE) || (class_interfaces == UX_NULL))
  {
    return USBD_DESCRIPTOR_ERROR;
  }

  /* Check if no more class available */
  if (device_handle.device_class_numbers >= USBD_MAX_SUPPORTED_CLASS)
  {
    return USBD_DESCRIPTOR_CLASS_FULL;
  }

  while (class_index < USBD_MAX_SUPPORTED_CLASS)
  {
    if (device_handle.device_class_list[class_index].device_class_id == 0)
    {
      device_handle.device_class_list[class_index].device_class_type = class_type;
      device_handle.device_class_list[class_index].interfaces = (USB_DEVICE_INTERFACE_HANDLE *) class_interfaces;

#if USBD_HID_CLASS_ACTIVATED == 1U
      if (device_handle.device_class_list[class_index].device_class_type == USBD_CLASS_TYPE_HID)
      {
        if (parameter == UX_NULL)
        {
          return USBD_DESCRIPTOR_ERROR;
        }

        device_handle.device_class_list[class_index].interfaces->other_parameter = (ULONG) *parameter;
      }
#endif /* USBD_HID_CLASS_ACTIVATED */

#if USBD_CDC_ACM_CLASS_ACTIVATED == 1U
      if (device_handle.device_class_list[class_index].device_class_type == USBD_CLASS_TYPE_CDC_ACM)
      {
        class_interfaces->next_interface = &class_interfaces[1];
      }
#endif /* USBD_CDC_ACM_CLASS_ACTIVATED */

#if USBD_VIDEO_CLASS_ACTIVATED == 1U
      if (device_handle.device_class_list[class_index].device_class_type == USBD_CLASS_TYPE_VIDEO)
      {
        class_interfaces->next_interface = &class_interfaces[1];
      }
#endif /* USBD_VIDEO_CLASS_ACTIVATED */

#if USBD_AUDIO_CLASS_ACTIVATED == 1U
      if ((device_handle.device_class_list[class_index].device_class_type == USBD_CLASS_TYPE_AUDIO_10) ||
          (device_handle.device_class_list[class_index].device_class_type == USBD_CLASS_TYPE_AUDIO_20))
      {
        class_interfaces->next_interface = &class_interfaces[1];
        class_interfaces[1].next_interface = &class_interfaces[2];
      }
#endif /* USBD_AUDIO_CLASS_ACTIVATED */

      device_handle.device_class_list[class_index].device_class_id = class_index + 1;
      device_handle.device_class_numbers += 1U;

      p_interface = device_handle.device_class_list[class_index].interfaces;

      /* Find the first interface available slot */
      for (itf_index = 0U; itf_index < USBD_MAX_CLASS_INTERFACES; itf_index++)
      {
        p_interface->interface_numbers = device_handle.device_class_total_itfs_num;

        device_handle.device_class_total_itfs_num += 1U;

        p_interface = p_interface->next_interface;

        if (p_interface == UX_NULL)
        {
          break;
        }
      }

      break;
    }

    class_index ++;
  }

  return USBD_DESCRIPTOR_SUCCESS;
}

/**
  * @brief  usb_device_add_string_descriptor
  *         Device add user string descriptor framework builder
  * @retval status
  */
UINT usb_device_add_string_descriptor(USBD_USER_STRING_DESC *user_string_desc)
{
  UCHAR                 user_sting_index = 0U;
  USBD_USER_STRING_DESC *p_user_string_desc;

  p_user_string_desc = (USBD_USER_STRING_DESC *) user_string_desc;

  while (user_sting_index < USBD_MAX_SUPPORTED_USER_STRING_DESC)
  {
    if (string_descriptor_handle.user_string_desc_list[user_sting_index].is_used == 0U)
    {
      string_descriptor_handle.user_string_desc_numbers ++;

      string_descriptor_handle.user_string_desc_list[user_sting_index].is_used = 1U;

      string_descriptor_handle.user_string_desc_list[user_sting_index].language_id = p_user_string_desc->language_id;
      string_descriptor_handle.user_string_desc_list[user_sting_index].string_index = p_user_string_desc->string_index;
      string_descriptor_handle.user_string_desc_list[user_sting_index].string_length = p_user_string_desc->string_length;
      string_descriptor_handle.user_string_desc_list[user_sting_index].string_descriptor = p_user_string_desc->string_descriptor;

      break;
    }

    user_sting_index ++;
  }

  return USBD_DESCRIPTOR_SUCCESS;
}

/**
  * @brief  usb_device_descriptor_get_framework
  * @retval status
  */
UINT usb_device_descriptor_get_framework(USB_DESCRIPTOR *usbd_descriptor)
{
  UINT status;
  static USB_DESCRIPTOR         *p_desc;
  static USBD_FRAMEWORK_HANDLE  *p_string_desc;
  static USBD_FRAMEWORK_HANDLE  *p_languageid_desc;
  static USBD_FRAMEWORK_HANDLE  *p_device_hs_desc;
  static USBD_FRAMEWORK_HANDLE  *p_device_fs_desc;

  if (usbd_descriptor == UX_NULL)
  {
    return USBD_DESCRIPTOR_ERROR;
  }

  p_desc = (USB_DESCRIPTOR *) usbd_descriptor;


  p_device_hs_desc = (USBD_FRAMEWORK_HANDLE *) &p_desc->device_high_speed;

#if USBD_HIGH_SPEED_SUPPORTED == 1U

  p_device_hs_desc->framework = (UCHAR*) usb_device_framework_high_speed;
  p_device_hs_desc->framework_length = 0U;

    /* Set USB device descriptor framework high speed pointer and reset length */
  status = usb_device_framework_builder(&device_handle, (UCHAR *) usb_device_framework_high_speed,
                                        &p_device_hs_desc->framework_length,
                                        UX_HIGH_SPEED_DEVICE);

  if (status != USBD_DESCRIPTOR_SUCCESS)
  {
    return status;
  }

#else /* USBD_HIGH_SPEED_SUPPORTED */

  p_device_hs_desc->framework = UX_NULL;
  p_device_hs_desc->framework_length = 0U;

#endif /* USBD_HIGH_SPEED_SUPPORTED */

  p_device_fs_desc = (USBD_FRAMEWORK_HANDLE *) &p_desc->device_full_speed;


  /* Set USB device descriptor framework full speed pointer and reset length */
  p_device_fs_desc->framework = (UCHAR*) usb_device_framework_full_speed;
  p_device_fs_desc->framework_length = 0U;

  /* Call USB device descriptor framework builder */
  status = usb_device_framework_builder(&device_handle, (UCHAR *) usb_device_framework_full_speed,
                                        &p_device_fs_desc->framework_length,
                                        UX_FULL_SPEED_DEVICE);

  if (status != USBD_DESCRIPTOR_SUCCESS)
  {
    return status;
  }

  p_string_desc =(USBD_FRAMEWORK_HANDLE *) &p_desc->string;

  /* Set USB device string framework full speed pointer and reset length */
  p_string_desc->framework = (UCHAR*) usb_device_string_framework;
  p_string_desc->framework_length = 0U;

  /* Call USB device string descriptor framework builder */
  status = usb_device_string_framework_builder(&string_descriptor_handle,
                                               usb_device_string_framework,
                                               &p_string_desc->framework_length);

  if (status != USBD_DESCRIPTOR_SUCCESS)
  {
    return status;
  }

  p_languageid_desc = (USBD_FRAMEWORK_HANDLE *) &p_desc->languageid;

  /* Set USB device language id framework pointer and reset length */
  p_languageid_desc->framework = (UCHAR*) usb_device_language_id_framework;
  p_languageid_desc->framework_length = 0U;

  /* Call USB device language id descriptor framework builder */
  status = usb_device_language_id_framework_builder(usb_device_language_id_framework,
                                                    &p_languageid_desc->framework_length);

  if (status != USBD_DESCRIPTOR_SUCCESS)
  {
    return status;
  }

  return USBD_DESCRIPTOR_SUCCESS;

}

/**
  * @brief  usb_device_framework_builder
  *         Device descriptor framework builder
  * @retval status
  */
static UINT usb_device_framework_builder(USB_DEVICE_HANDLE *p_device, UCHAR *framework_pointer,
                                         ULONG *framework_length, UCHAR device_speed)
{
  UINT status = USBD_DESCRIPTOR_SUCCESS;
  UCHAR class_index = 0U;
  static USB_DEVICE_DESC                *p_device_desc = UX_NULL;
#if USBD_HIGH_SPEED_SUPPORTED == 1U
  static USBD_QUALIFIER_DESC            *p_device_qualifier_desc = UX_NULL;
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  static USBD_CONFIGURATION_DESC        *p_device_configuration_desc = UX_NULL;
  USB_DEVICE_COMPOSITE_HANDLE           *p_device_class;
  USB_DEVICE_INTERFACE_HANDLE           *p_interface;
  ULONG device_framework_size = 0U;
  ULONG config_framework_size = 0U;
  static UCHAR *config_framework_pointer;
  static UCHAR *device_framework_pointer;

  device_framework_pointer = (UCHAR *) framework_pointer;

  /* Start building the generic device descriptor common part */
  p_device_desc = (USB_DEVICE_DESC *)device_framework_pointer;
  p_device_desc->bLength                = (UCHAR)sizeof(USB_DEVICE_DESC);
  p_device_desc->bDescriptorType        = UX_DEVICE_DESCRIPTOR_ITEM;
  p_device_desc->bcdUSB                 = USBD_BCDUSB;
  p_device_desc->bDeviceClass           = USBD_CLASS;
  p_device_desc->bDeviceSubClass        = USBD_SUBCLASS;
  p_device_desc->bDeviceProtocol        = USBD_PROTOCOL;
  p_device_desc->bMaxPacketSize0        = USBD_MAX_PACKET_SIZE_EP0;
  p_device_desc->idVendor               = USBD_VENDOR_ID;
  p_device_desc->idProduct              = USBD_PRODUCT_ID;
  p_device_desc->bcdDevice              = USBD_BCD_DEVICE;
  p_device_desc->iManufacturer          = USBD_IDX_MANUFACTURE_STR;
  p_device_desc->iProduct               = USBD_IDX_PRODUCT_STR;
  p_device_desc->iSerialNumber          = USBD_IDX_SERIAL_STR;
  p_device_desc->bNumConfigurations = USBD_MAX_NUM_CONFIGURATION;
  device_framework_size += (ULONG)sizeof(USB_DEVICE_DESC);

  if (p_device->device_class_total_itfs_num > 1)
  {
    p_device_desc->bDeviceClass = USBD_COMPOSITE_CLASS;
    p_device_desc->bDeviceSubClass = USBD_COMPOSITE_SUBCLASS;
    p_device_desc->bDeviceProtocol = USBD_COMPOSITE_PROTOCOL;
  }
#if USBD_CDC_ACM_CLASS_ACTIVATED == 1U
  else
  {
    if (p_device->device_class_list[0].device_class_type == USBD_CLASS_TYPE_CDC_ACM)
    {
      p_device_desc->bDeviceClass = USBD_CDCACM_CLASS;
      p_device_desc->bDeviceSubClass = USBD_CDCACM_SUBCLASS;
      p_device_desc->bDeviceProtocol = USBD_CDCACM_PROTOCOL;
    }
  }
#endif /* USBD_CDC_ACM_CLASS_ACTIVATED */

#if USBD_HIGH_SPEED_SUPPORTED == 1U

  /* Add qualifier descriptor only for high speed framework */
  if (device_speed == UX_HIGH_SPEED_DEVICE)
  {
    p_device_qualifier_desc = (USBD_QUALIFIER_DESC *)(device_framework_pointer + device_framework_size);
    p_device_qualifier_desc->bLength            = (UCHAR)sizeof(USBD_QUALIFIER_DESC);
    p_device_qualifier_desc->bDescriptorType    = UX_DEVICE_QUALIFIER_DESCRIPTOR_ITEM;
    p_device_qualifier_desc->bcdUSB             = USBD_BCDUSB;
    p_device_qualifier_desc->bDeviceClass       = USBD_CLASS;
    p_device_qualifier_desc->bDeviceSubClass    = USBD_SUBCLASS;
    p_device_qualifier_desc->bDeviceProtocol    = USBD_PROTOCOL;
    p_device_qualifier_desc->bMaxPacketSize0    = USBD_MAX_PACKET_SIZE_EP0;
    p_device_qualifier_desc->bNumConfigurations = USBD_MAX_NUM_CONFIGURATION;
    p_device_qualifier_desc->bReserved          = USBD_RESERVED;
    device_framework_size += (ULONG)sizeof(USBD_QUALIFIER_DESC);

    if (p_device->device_class_total_itfs_num > 1)
    {
      p_device_qualifier_desc->bDeviceClass     = USBD_COMPOSITE_CLASS;
      p_device_qualifier_desc->bDeviceSubClass  = USBD_COMPOSITE_SUBCLASS;
      p_device_qualifier_desc->bDeviceProtocol  = USBD_COMPOSITE_PROTOCOL;
    }
#if USBD_CDC_ACM_CLASS_ACTIVATED == 1U
    else
    {
      if (p_device->device_class_list[0].device_class_type == USBD_CLASS_TYPE_CDC_ACM)
      {
        p_device_qualifier_desc->bDeviceClass     = USBD_CDCACM_CLASS;
        p_device_qualifier_desc->bDeviceSubClass  = USBD_CDCACM_SUBCLASS;
        p_device_qualifier_desc->bDeviceProtocol  = USBD_CDCACM_PROTOCOL;
      }
    }
#endif /* USBD_CDC_ACM_CLASS_ACTIVATED */
  }
#endif /* USBD_HIGH_SPEED_SUPPORTED */

  config_framework_pointer = device_framework_pointer + device_framework_size;

  /* Add a new class to the configuration descriptor */
  p_device_configuration_desc = (USBD_CONFIGURATION_DESC *)(config_framework_pointer);
  p_device_configuration_desc->bLength                  = (UCHAR)sizeof(USBD_CONFIGURATION_DESC);
  p_device_configuration_desc->bDescriptorType          = UX_CONFIGURATION_DESCRIPTOR_ITEM;
  p_device_configuration_desc->wTotalLength             = USBD_CONFIG_TOTAL_LENGTH;
  p_device_configuration_desc->bNumInterfaces           = USBD_NUM_INTERFACES;
  p_device_configuration_desc->bConfigurationValue      = USBD_CONFIG_VALUE;
  p_device_configuration_desc->iConfiguration           = USBD_CONFIG_STR_DESC_IDX;
  p_device_configuration_desc->bmAttributes             = USBD_CONFIG_BMATTRIBUTES;
  p_device_configuration_desc->bMaxPower                = USBD_CONFIG_MAXPOWER;
  config_framework_size += (ULONG)sizeof(USBD_CONFIGURATION_DESC);

  /* Parse class in usb handle and builder class descriptor framework */
  for (class_index = 0; class_index < USBD_MAX_SUPPORTED_CLASS; class_index++)
  {
    p_device_class = (USB_DEVICE_COMPOSITE_HANDLE *) (device_handle.device_class_list + class_index);

    /* Check if class type equal to none return */
    if (p_device_class->device_class_type == USBD_CLASS_TYPE_NONE)
    {
      break;
    }

    p_interface = p_device_class->interfaces;

    /* Check class type then configure and append to the descriptor framework */
    switch(p_device_class->device_class_type)
    {

#if USBD_HID_CLASS_ACTIVATED == 1U
      case USBD_CLASS_TYPE_HID:
        /* Call USB HID device class descriptor framework builder */
        status = usb_device_hid_framework_builder(p_interface, (ULONG) config_framework_pointer,
                                                  &config_framework_size, device_speed);
        break;
#endif /* USBD_HID_CLASS_ACTIVATED */

#if USBD_CDC_ACM_CLASS_ACTIVATED == 1
      case USBD_CLASS_TYPE_CDC_ACM:
        /* Call USB CDC_ACM device class descriptor framework builder */
        status = usb_device_cdc_acm_framework_builder(p_interface, (ULONG) config_framework_pointer,
                                                      &config_framework_size, device_speed);
        break;
#endif /* USBD_CDC_ACM_CLASS_ACTIVATED */

#if USBD_MSC_CLASS_ACTIVATED == 1U
      case USBD_CLASS_TYPE_MSC:
        status = usb_device_msc_framework_builder(p_interface, (ULONG) config_framework_pointer,
                  &config_framework_size, device_speed);
        break;
#endif /* USBD_MSC_CLASS_ACTIVATED */

#if USBD_DFU_CLASS_ACTIVATED == 1U
      case USBD_CLASS_TYPE_DFU:
        status = usb_device_dfu_framework_builder(p_interface, (ULONG) config_framework_pointer,
                  &config_framework_size, device_speed);
        break;
#endif /* USBD_DFU_CLASS_ACTIVATED */

#if USBD_MTP_CLASS_ACTIVATED == 1
      case USBD_CLASS_TYPE_PIMA_MTP:
        /* Call USB MTP device class descriptor framework builder */
        status = usb_device_mtp_framework_builder(p_interface, (ULONG) config_framework_pointer,
                                                  &config_framework_size, device_speed);
        break;
#endif /* USBD_MTP_CLASS_ACTIVATED */

#if USBD_AUDIO_CLASS_ACTIVATED == 1U
      case USBD_CLASS_TYPE_AUDIO_10:
      case USBD_CLASS_TYPE_AUDIO_20:
        status = usb_device_audio_framework_builder(p_interface, (ULONG) config_framework_pointer,
                    &config_framework_size, device_speed);
        break;
#endif /* USBD_AUDIO_CLASS_ACTIVATED */

#if USBD_VIDEO_CLASS_ACTIVATED == 1
      case USBD_CLASS_TYPE_VIDEO:
        /* Call USB VIDEO device class descriptor framework builder */
        status = usb_device_video_framework_builder(p_interface, (ULONG) config_framework_pointer,
                                                    &config_framework_size, device_speed);
        break;
#endif /* USBD_VIDEO_CLASS_ACTIVATED */

#if USBD_PRINTER_CLASS_ACTIVATED == 1U
      case USBD_CLASS_TYPE_PRINTER:
        status = usb_device_printer_framework_builder(p_interface, (ULONG) config_framework_pointer,
                  &config_framework_size, device_speed);
        break;
#endif /* USBD_PRINTER_CLASS_ACTIVATED */

#if USBD_CCID_CLASS_ACTIVATED == 1U
      case USBD_CLASS_TYPE_CCID:
        status = usb_device_ccid_framework_builder(p_interface, (ULONG) config_framework_pointer,
                   &config_framework_size, device_speed);
        break;
#endif /* USBD_CCID_CLASS_ACTIVATED */

      default:
        UX_PARAMETER_NOT_USED(p_interface);
        status = USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
        break;
    }

    if (status != USBD_DESCRIPTOR_SUCCESS)
    {
      return status;
    }
  }

  *framework_length = device_framework_size + config_framework_size;

#if USBD_HIGH_SPEED_SUPPORTED == 1U

  if ((device_speed == UX_HIGH_SPEED_DEVICE) && (*framework_length > USBD_FRAMEWORK_HS_MAX_LENGTH))
  {
    return USBD_DESCRIPTOR_MEMORY_INSUFFICIENT;
  }

#endif /* USBD_HIGH_SPEED_SUPPORTED */

  if ((device_speed == UX_FULL_SPEED_DEVICE) && (*framework_length > USBD_FRAMEWORK_FS_MAX_LENGTH))
  {
    return USBD_DESCRIPTOR_MEMORY_INSUFFICIENT;
  }

  return status;
}

/**
  * @brief  usb_device_string_framework_builder
  *         Device string descriptor framework builder
  * @retval status
  */
static UINT usb_device_string_framework_builder(USBD_USER_STRING_DESC_HANDLE *p_string_descriptor,
                                                UCHAR* framework_pointer, ULONG *framework_length)
{
  UCHAR                 user_string_index;
  UCHAR                 *p_string_framework = UX_NULL;
  USB_STRING_DESC       *p_manufacturer_string_desc = UX_NULL;
  USB_STRING_DESC       *p_product_string_desc = UX_NULL;
  USB_STRING_DESC       *p_serial_number_string_desc = UX_NULL;
  USB_STRING_DESC       *p_string_desc = UX_NULL;
  USBD_USER_STRING_DESC_HANDLE  *user_string_desc;

  user_string_desc = (USBD_USER_STRING_DESC_HANDLE *) p_string_descriptor;

  p_string_framework = (UCHAR *) framework_pointer;

  /* Set USBD_IDX_MANUFACTURE_STR and USBD_MANUFACTURER_STRING in string_framework */
  p_manufacturer_string_desc = (USB_STRING_DESC *) p_string_framework;
  p_manufacturer_string_desc->language_id       = USBD_LANG_ID_STRING;
  p_manufacturer_string_desc->string_index      = USBD_IDX_MANUFACTURE_STR;
  p_manufacturer_string_desc->string_length     = ux_utility_string_length_get((UCHAR *)USBD_MANUFACTURER_STRING);
  *framework_length += (ULONG) sizeof(USB_STRING_DESC);

  ux_utility_memory_copy(p_string_framework + *framework_length,
                         USBD_MANUFACTURER_STRING,
                         p_manufacturer_string_desc->string_length);

  *framework_length += (ULONG) p_manufacturer_string_desc->string_length;

  /* Set USBD_IDX_PRODUCT_STR and USBD_PRODUCT_STRING in string_framework */
  p_product_string_desc = (USB_STRING_DESC *) (p_string_framework + *framework_length);
  p_product_string_desc->language_id    = USBD_LANG_ID_STRING;
  p_product_string_desc->string_index   = USBD_IDX_PRODUCT_STR;
  p_product_string_desc->string_length  = ux_utility_string_length_get((UCHAR *)USBD_PRODUCT_STRING);
  *framework_length += (ULONG) sizeof(USB_STRING_DESC);

  ux_utility_memory_copy(p_string_framework + *framework_length,
                         USBD_PRODUCT_STRING,
                         p_product_string_desc->string_length);

  *framework_length += (ULONG) p_product_string_desc->string_length;

  /* Set USBD_IDX_SERIAL_STR and USBD_SERIAL_NUMBER_STRING in string_framework */
  p_serial_number_string_desc = (USB_STRING_DESC *) (p_string_framework + *framework_length);
  p_serial_number_string_desc->language_id      = USBD_LANG_ID_STRING;
  p_serial_number_string_desc->string_index     = USBD_IDX_SERIAL_STR;
  p_serial_number_string_desc->string_length    = ux_utility_string_length_get((UCHAR *)USBD_SERIAL_NUMBER_STRING);
  *framework_length += (ULONG) sizeof(USB_STRING_DESC);

  ux_utility_memory_copy(p_string_framework + *framework_length,
                         USBD_SERIAL_NUMBER_STRING,
                         p_serial_number_string_desc->string_length);

  *framework_length += (ULONG) p_serial_number_string_desc->string_length;

#if USBD_DFU_CLASS_ACTIVATED == 1U
#if defined(USBD_DFU_STRING_DESC_INDEX) && defined(USBD_DFU_STRING_DESC)
  p_string_desc = (USB_STRING_DESC *) (p_string_framework + *framework_length);
  p_string_desc->language_id    = USBD_LANG_ID_STRING;
  p_string_desc->string_index   = USBD_DFU_STRING_DESC_INDEX;
  p_string_desc->string_length  = ux_utility_string_length_get((UCHAR *)USBD_DFU_STRING_DESC);
  *framework_length += (ULONG) sizeof(USB_STRING_DESC);

  if ((*framework_length + p_string_desc->string_length) > USBD_STRING_FRAMEWORK_MAX_LENGTH)
  {
    return USBD_DESCRIPTOR_MEMORY_INSUFFICIENT;
  }

  ux_utility_memory_copy(p_string_framework + *framework_length,
                         USBD_DFU_STRING_DESC,
                         p_string_desc->string_length);

  *framework_length += (ULONG) p_string_desc->string_length;
#endif
#endif /* USBD_DFU_CLASS_ACTIVATED */

  /* Parse user string descriptors */
  for (user_string_index = 0U; user_string_index < user_string_desc->user_string_desc_numbers; user_string_index++)
  {
    if (user_string_desc->user_string_desc_list[user_string_index].is_used == 1U)
    {
      /* Set USB Device user string descriptor index and string descriptor in string_framework */
      p_string_desc = (USB_STRING_DESC *) (p_string_framework + *framework_length);
      p_string_desc->language_id    = user_string_desc->user_string_desc_list[user_string_index].language_id;
      p_string_desc->string_index   = user_string_desc->user_string_desc_list[user_string_index].string_index;
      p_string_desc->string_length  = user_string_desc->user_string_desc_list[user_string_index].string_length;
      *framework_length += (ULONG) sizeof(USB_STRING_DESC);

      if ((*framework_length + p_string_desc->string_length)  > USBD_STRING_FRAMEWORK_MAX_LENGTH)
      {
        return USBD_DESCRIPTOR_MEMORY_INSUFFICIENT;
      }

      ux_utility_memory_copy(p_string_framework + *framework_length,
                             user_string_desc->user_string_desc_list[user_string_index].string_descriptor,
                             p_string_desc->string_length);

      *framework_length += (ULONG) p_string_desc->string_length;
    }
  }

  if (*framework_length > USBD_STRING_FRAMEWORK_MAX_LENGTH)
  {
    return USBD_DESCRIPTOR_MEMORY_INSUFFICIENT;
  }

  return USBD_DESCRIPTOR_SUCCESS;
}

/**
  * @brief  usb_device_language_id_framework_builder
  *         Device language id descriptor framework builder
  * @retval status
  */
static UINT usb_device_language_id_framework_builder(UCHAR* framework_pointer, ULONG *framework_length)
{
  UCHAR  *p_language_id_framework = UX_NULL;
  USB_DEVICE_LANGUAGE_ID_DESC *p_language_id = UX_NULL;

  p_language_id_framework = (UCHAR *) framework_pointer;

  p_language_id = (USB_DEVICE_LANGUAGE_ID_DESC *)p_language_id_framework;
  p_language_id->language_id = USBD_LANG_ID_STRING;
  *framework_length = (ULONG) sizeof(USB_DEVICE_LANGUAGE_ID_DESC);

  if (*framework_length > USBD_LANGUAGE_ID_MAX_LENGTH)
  {
    return USBD_DESCRIPTOR_MEMORY_INSUFFICIENT;
  }

  return USBD_DESCRIPTOR_SUCCESS;
}

UINT usb_device_framework_set_interface(ULONG p_config_framework,
                                        ULONG *config_framework_size,
                                        USBD_INTERFACE_DESC *p_interface_desc,
                                        USB_DEVICE_INTERFACE_HANDLE *p_interface)
{
  p_interface_desc = (USBD_INTERFACE_DESC*)((ULONG)p_config_framework + *config_framework_size);
  p_interface_desc->bLength = (UCHAR)sizeof(USBD_INTERFACE_DESC);
  p_interface_desc->bDescriptorType     = UX_INTERFACE_DESCRIPTOR_ITEM;
  p_interface_desc->bInterfaceNumber    = p_interface->interface_numbers;
  p_interface_desc->bAlternateSetting   = p_interface->alternate_setting;
  p_interface_desc->bNumEndPoints       = p_interface->endpoints_numbers;
  p_interface_desc->bInterfaceClass     = p_interface->interface_class;
  p_interface_desc->bInterfaceSubClass  = p_interface->interface_subclass;
  p_interface_desc->bInterfaceProtocol  = p_interface->interface_protocol;
  p_interface_desc->iInterface          = p_interface->i_interface;
  *config_framework_size += (ULONG)sizeof(USBD_INTERFACE_DESC);

  return USBD_DESCRIPTOR_SUCCESS;
}

UINT usb_device_framework_set_endpoint(ULONG p_config_framework,
                                       ULONG *config_framework_size,
                                       USBD_ENDPOINT_DESC *p_endpoint_desc,
                                       USB_DEVICE_ENDPOINT_HANDLE *p_endpoint,
                                       UCHAR device_speed)
{
  p_endpoint_desc = (USBD_ENDPOINT_DESC*)((ULONG)p_config_framework + *config_framework_size);
  p_endpoint_desc->bLength              = (UCHAR)sizeof(USBD_ENDPOINT_DESC);
  p_endpoint_desc->bDescriptorType      = UX_ENDPOINT_DESCRIPTOR_ITEM;
  p_endpoint_desc->bEndpointAddress     = p_endpoint->endpoint_add;
  p_endpoint_desc->bmAttributes         = p_endpoint->endpoint_type;

#if USBD_HIGH_SPEED_SUPPORTED == 1
  /* Check the current speed to set endpoint wMaxPacketSize and bInterval */
  if (device_speed == UX_HIGH_SPEED_DEVICE)
  {
    p_endpoint_desc->wMaxPacketSize     = p_endpoint -> endpoint_size_hs;
    p_endpoint_desc->bInterval          = p_endpoint -> endpoint_size_binterval_hs;
  }
  else
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  {
    p_endpoint_desc->wMaxPacketSize     = p_endpoint->endpoint_size_fs;
    p_endpoint_desc->bInterval          = p_endpoint->endpoint_size_binterval_fs;
  }

  *config_framework_size += (ULONG)sizeof(USBD_ENDPOINT_DESC);

  return USBD_DESCRIPTOR_SUCCESS;
}

#if USBD_MSC_CLASS_ACTIVATED == 1U
/**
  * @brief  usb_device_msc_framework_builder
  *         Configure and Append the MSC Descriptor
  * @retval none
  */
static UINT usb_device_msc_framework_builder(USB_DEVICE_INTERFACE_HANDLE *p_interface,
                                             ULONG p_config_framework,
                                             ULONG *config_framework_size,
                                             UCHAR device_speed)
{
  UCHAR                       interface_index = 0U;
  USBD_INTERFACE_DESC         *p_msc_itf_desc = UX_NULL;
  USBD_ENDPOINT_DESC          *p_msc_ep_desc = UX_NULL;
  USB_DEVICE_INTERFACE_HANDLE *p_msc_interface = UX_NULL;
  USB_DEVICE_ENDPOINT_HANDLE  *p_msc_in_endpoint = UX_NULL;
  USB_DEVICE_ENDPOINT_HANDLE  *p_msc_out_endpoint = UX_NULL;

  p_msc_interface = (USB_DEVICE_INTERFACE_HANDLE *)p_interface;

  if (p_msc_interface == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  usb_device_framework_set_interface((ULONG) p_config_framework, config_framework_size,
                                     p_msc_itf_desc, p_msc_interface);

  interface_index++;

  p_msc_in_endpoint = (USB_DEVICE_ENDPOINT_HANDLE *)p_msc_interface->device_endpoint;

  if (p_msc_in_endpoint == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  usb_device_framework_set_endpoint((ULONG) p_config_framework, config_framework_size,
                                    p_msc_ep_desc, p_msc_in_endpoint, device_speed);

  if (p_msc_interface->endpoints_numbers > 1U)
  {
    p_msc_in_endpoint->next_endpoint = &p_msc_interface->device_endpoint[1];
    p_msc_out_endpoint = (USB_DEVICE_ENDPOINT_HANDLE *)p_msc_in_endpoint->next_endpoint;

    if (p_msc_out_endpoint == UX_NULL)
    {
      return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
    }

    usb_device_framework_set_endpoint((ULONG) p_config_framework, config_framework_size,
                                      p_msc_ep_desc, p_msc_out_endpoint, device_speed);
  }

  ((USBD_CONFIGURATION_DESC *)p_config_framework)->wTotalLength = *config_framework_size;
  ((USBD_CONFIGURATION_DESC *)p_config_framework)->bNumInterfaces += interface_index;

  return USBD_DESCRIPTOR_SUCCESS;
}
#endif /* USBD_MSC_CLASS_ACTIVATED */

#if USBD_DFU_CLASS_ACTIVATED == 1U
/**
  * @brief  usb_device_dfu_framework_builder
  *         Configure and Append the DFU Descriptor
  * @retval none
  */
static UINT usb_device_dfu_framework_builder(USB_DEVICE_INTERFACE_HANDLE *p_interface,
                                             ULONG p_config_framework,
                                             ULONG *config_framework_size,
                                             UCHAR device_speed)
{
  UCHAR interface_index = 0U;
  USBD_INTERFACE_DESC *p_dfu_itf_desc = UX_NULL;
  USBD_DFU_FUNCTIONAL_DESC *p_dfu_desc = UX_NULL;
  USB_DEVICE_INTERFACE_HANDLE *p_dfu_interface = UX_NULL;

  UX_PARAMETER_NOT_USED(device_speed);

  p_dfu_interface = (USB_DEVICE_INTERFACE_HANDLE *) p_interface;

  if (p_dfu_interface == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  usb_device_framework_set_interface((ULONG) p_config_framework, config_framework_size,
                                     p_dfu_itf_desc, p_dfu_interface);

  interface_index++;

  p_dfu_desc = (USBD_DFU_FUNCTIONAL_DESC *) ((ULONG) p_config_framework + *config_framework_size);
  p_dfu_desc->bLength = (UCHAR) sizeof(USBD_DFU_FUNCTIONAL_DESC);
  p_dfu_desc->bDescriptorType = USBD_DFU_DESCRIPTOR_TYPE;
  p_dfu_desc->bmAttributes = USBD_DFU_BM_ATTRIBUTES;
  p_dfu_desc->wDetachTimeOut = USBD_DFU_DETACH_TIMEOUT;
  p_dfu_desc->wTransferSize = USBD_DFU_XFER_SIZE;
  p_dfu_desc->bcdDFUVersion = USBD_DFU_BCD_VERSION;
  *config_framework_size += (ULONG) sizeof(USBD_DFU_FUNCTIONAL_DESC);

  ((USBD_CONFIGURATION_DESC *)p_config_framework)->wTotalLength = *config_framework_size;
  ((USBD_CONFIGURATION_DESC *)p_config_framework)->bNumInterfaces += interface_index;

  return USBD_DESCRIPTOR_SUCCESS;
}
#endif /* USBD_DFU_CLASS_ACTIVATED */

#if USBD_AUDIO_CLASS_ACTIVATED == 1U
/**
  * @brief  usb_device_audio_framework_builder
  *         Configure and Append the AUDIO Descriptor
  * @retval none
  */
static UINT usb_device_audio_framework_builder(USB_DEVICE_INTERFACE_HANDLE *p_interface,
                                               ULONG p_config_framework,
                                               ULONG *config_framework_size,
                                               UCHAR device_speed)
{
  UCHAR interface_index = 0U;
  UCHAR stream_interface_number = 0U;
  USBD_INTERFACE_DESC *p_audio_itf_desc = UX_NULL;
  USBD_ENDPOINT_DESC *p_audio_ep_desc = UX_NULL;
  USBD_AUDIO_AC_CS_IF_DESC *p_audio_ac_desc = UX_NULL;
  USBD_AUDIO_CLOCK_SOURCE_DESC *p_audio_clock_desc = UX_NULL;
  USBD_AUDIO_INPUT_TERMINAL_DESC *p_audio_input_terminal_desc = UX_NULL;
  USBD_AUDIO_FEATURE_UNIT_DESC *p_audio_feature_desc = UX_NULL;
  USBD_AUDIO_OUTPUT_TERMINAL_DESC *p_audio_output_terminal_desc = UX_NULL;
  USBD_AUDIO_AS_CS_IF_DESC *p_audio_as_desc = UX_NULL;
  USBD_AUDIO_FORMAT_TYPE_DESC *p_audio_format_desc = UX_NULL;
  USBD_AUDIO_CS_EP_DESC *p_audio_cs_ep_desc = UX_NULL;
  USB_DEVICE_INTERFACE_HANDLE *p_audio_control_interface = UX_NULL;
  USB_DEVICE_INTERFACE_HANDLE *p_audio_stream_in_interface = UX_NULL;
  USB_DEVICE_INTERFACE_HANDLE *p_audio_stream_out_interface = UX_NULL;
  USB_DEVICE_INTERFACE_HANDLE *p_audio_stream_interface_alt = UX_NULL;
  USB_DEVICE_ENDPOINT_HANDLE *p_audio_control_endpoint = UX_NULL;
  USB_DEVICE_ENDPOINT_HANDLE *p_audio_endpoint = UX_NULL;

  UX_PARAMETER_NOT_USED(device_speed);

  p_audio_control_interface = (USB_DEVICE_INTERFACE_HANDLE *) p_interface;

  if ((p_audio_control_interface == UX_NULL) ||
      (p_audio_control_interface->next_interface == UX_NULL) ||
      (p_audio_control_interface->next_interface->next_interface == UX_NULL))
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  p_audio_stream_in_interface = p_audio_control_interface->next_interface;
  p_audio_stream_out_interface = p_audio_stream_in_interface->next_interface;

  usb_device_framework_set_interface((ULONG) p_config_framework, config_framework_size,
                                     p_audio_itf_desc, p_audio_control_interface);

  interface_index++;

  p_audio_ac_desc = (USBD_AUDIO_AC_CS_IF_DESC *) ((ULONG) p_config_framework + *config_framework_size);
  p_audio_ac_desc->bLength = (UCHAR) sizeof(USBD_AUDIO_AC_CS_IF_DESC);
  p_audio_ac_desc->bDescriptorType = 0x24U;
  p_audio_ac_desc->bDescriptorSubtype = 0x01U;
  p_audio_ac_desc->bcdADC = 0x0200U;
  p_audio_ac_desc->bCategory = USBD_AUDIO_CATEGORY;
  p_audio_ac_desc->wTotalLength = USBD_AUDIO_CONTROL_INTERFACE_SIZE;
  p_audio_ac_desc->bmControls = 0x00U;
  *config_framework_size += (ULONG) sizeof(USBD_AUDIO_AC_CS_IF_DESC);

  p_audio_clock_desc = (USBD_AUDIO_CLOCK_SOURCE_DESC *) ((ULONG) p_config_framework + *config_framework_size);
  p_audio_clock_desc->bLength = (UCHAR) sizeof(USBD_AUDIO_CLOCK_SOURCE_DESC);
  p_audio_clock_desc->bDescriptorType = 0x24U;
  p_audio_clock_desc->bDescriptorSubtype = 0x0AU;
  p_audio_clock_desc->bClockID = USBD_AUDIO_CLOCK_SOURCE_ID;
  p_audio_clock_desc->bmAttributes = 0x01U;
  p_audio_clock_desc->bmControls = 0x01U;
  p_audio_clock_desc->bAssocTerminal = 0x00U;
  p_audio_clock_desc->iClockSource = 0x00U;
  *config_framework_size += (ULONG) sizeof(USBD_AUDIO_CLOCK_SOURCE_DESC);

  p_audio_input_terminal_desc = (USBD_AUDIO_INPUT_TERMINAL_DESC *) ((ULONG) p_config_framework + *config_framework_size);
  p_audio_input_terminal_desc->bLength = (UCHAR) sizeof(USBD_AUDIO_INPUT_TERMINAL_DESC);
  p_audio_input_terminal_desc->bDescriptorType = 0x24U;
  p_audio_input_terminal_desc->bDescriptorSubtype = 0x02U;
  p_audio_input_terminal_desc->bTerminalID = USBD_AUDIO_CAPTURE_TERMINAL_INPUT_ID;
  p_audio_input_terminal_desc->wTerminalType = 0x0201U;
  p_audio_input_terminal_desc->bAssocTerminal = 0x00U;
  p_audio_input_terminal_desc->bCSourceID = USBD_AUDIO_CLOCK_SOURCE_ID;
  p_audio_input_terminal_desc->bNrChannels = USBD_AUDIO_CHANNEL_COUNT;
  p_audio_input_terminal_desc->bmChannelConfig = USBD_AUDIO_CHANNEL_MAP;
  p_audio_input_terminal_desc->iChannelNames = 0x00U;
  p_audio_input_terminal_desc->bmControls = 0x0000U;
  p_audio_input_terminal_desc->iTerminal = 0x00U;
  *config_framework_size += (ULONG) sizeof(USBD_AUDIO_INPUT_TERMINAL_DESC);

  p_audio_feature_desc = (USBD_AUDIO_FEATURE_UNIT_DESC *) ((ULONG) p_config_framework + *config_framework_size);
  p_audio_feature_desc->bLength = (UCHAR) sizeof(USBD_AUDIO_FEATURE_UNIT_DESC);
  p_audio_feature_desc->bDescriptorType = 0x24U;
  p_audio_feature_desc->bDescriptorSubtype = 0x06U;
  p_audio_feature_desc->bUnitID = USBD_AUDIO_CAPTURE_FEATURE_UNIT_ID;
  p_audio_feature_desc->bSourceID = USBD_AUDIO_CAPTURE_TERMINAL_INPUT_ID;
  p_audio_feature_desc->bmaControls[0] = (ULONG) (USBD_AUDIO_FU_CONTROL_MUTE | USBD_AUDIO_FU_CONTROL_VOLUME);
  p_audio_feature_desc->bmaControls[1] = 0x00000000UL;
  p_audio_feature_desc->bmaControls[2] = 0x00000000UL;
  p_audio_feature_desc->iFeature = 0x00U;
  *config_framework_size += (ULONG) sizeof(USBD_AUDIO_FEATURE_UNIT_DESC);

  p_audio_output_terminal_desc = (USBD_AUDIO_OUTPUT_TERMINAL_DESC *) ((ULONG) p_config_framework + *config_framework_size);
  p_audio_output_terminal_desc->bLength = (UCHAR) sizeof(USBD_AUDIO_OUTPUT_TERMINAL_DESC);
  p_audio_output_terminal_desc->bDescriptorType = 0x24U;
  p_audio_output_terminal_desc->bDescriptorSubtype = 0x03U;
  p_audio_output_terminal_desc->bTerminalID = USBD_AUDIO_CAPTURE_TERMINAL_OUTPUT_ID;
  p_audio_output_terminal_desc->wTerminalType = 0x0101U;
  p_audio_output_terminal_desc->bAssocTerminal = 0x00U;
  p_audio_output_terminal_desc->bSourceID = USBD_AUDIO_CAPTURE_FEATURE_UNIT_ID;
  p_audio_output_terminal_desc->bCSourceID = USBD_AUDIO_CLOCK_SOURCE_ID;
  p_audio_output_terminal_desc->bmaControls = 0x0000U;
  p_audio_output_terminal_desc->iTerminal = 0x00U;
  *config_framework_size += (ULONG) sizeof(USBD_AUDIO_OUTPUT_TERMINAL_DESC);

  p_audio_input_terminal_desc = (USBD_AUDIO_INPUT_TERMINAL_DESC *) ((ULONG) p_config_framework + *config_framework_size);
  p_audio_input_terminal_desc->bLength = (UCHAR) sizeof(USBD_AUDIO_INPUT_TERMINAL_DESC);
  p_audio_input_terminal_desc->bDescriptorType = 0x24U;
  p_audio_input_terminal_desc->bDescriptorSubtype = 0x02U;
  p_audio_input_terminal_desc->bTerminalID = USBD_AUDIO_PLAY_TERMINAL_INPUT_ID;
  p_audio_input_terminal_desc->wTerminalType = 0x0101U;
  p_audio_input_terminal_desc->bAssocTerminal = 0x00U;
  p_audio_input_terminal_desc->bCSourceID = USBD_AUDIO_CLOCK_SOURCE_ID;
  p_audio_input_terminal_desc->bNrChannels = USBD_AUDIO_CHANNEL_COUNT;
  p_audio_input_terminal_desc->bmChannelConfig = USBD_AUDIO_CHANNEL_MAP;
  p_audio_input_terminal_desc->iChannelNames = 0x00U;
  p_audio_input_terminal_desc->bmControls = 0x0000U;
  p_audio_input_terminal_desc->iTerminal = 0x00U;
  *config_framework_size += (ULONG) sizeof(USBD_AUDIO_INPUT_TERMINAL_DESC);

  p_audio_feature_desc = (USBD_AUDIO_FEATURE_UNIT_DESC *) ((ULONG) p_config_framework + *config_framework_size);
  p_audio_feature_desc->bLength = (UCHAR) sizeof(USBD_AUDIO_FEATURE_UNIT_DESC);
  p_audio_feature_desc->bDescriptorType = 0x24U;
  p_audio_feature_desc->bDescriptorSubtype = 0x06U;
  p_audio_feature_desc->bUnitID = USBD_AUDIO_PLAY_FEATURE_UNIT_ID;
  p_audio_feature_desc->bSourceID = USBD_AUDIO_PLAY_TERMINAL_INPUT_ID;
  p_audio_feature_desc->bmaControls[0] = (ULONG) (USBD_AUDIO_FU_CONTROL_MUTE | USBD_AUDIO_FU_CONTROL_VOLUME);
  p_audio_feature_desc->bmaControls[1] = 0x00000000UL;
  p_audio_feature_desc->bmaControls[2] = 0x00000000UL;
  p_audio_feature_desc->iFeature = 0x00U;
  *config_framework_size += (ULONG) sizeof(USBD_AUDIO_FEATURE_UNIT_DESC);

  p_audio_output_terminal_desc = (USBD_AUDIO_OUTPUT_TERMINAL_DESC *) ((ULONG) p_config_framework + *config_framework_size);
  p_audio_output_terminal_desc->bLength = (UCHAR) sizeof(USBD_AUDIO_OUTPUT_TERMINAL_DESC);
  p_audio_output_terminal_desc->bDescriptorType = 0x24U;
  p_audio_output_terminal_desc->bDescriptorSubtype = 0x03U;
  p_audio_output_terminal_desc->bTerminalID = USBD_AUDIO_PLAY_TERMINAL_OUTPUT_ID;
  p_audio_output_terminal_desc->wTerminalType = 0x0301U;
  p_audio_output_terminal_desc->bAssocTerminal = 0x00U;
  p_audio_output_terminal_desc->bSourceID = USBD_AUDIO_PLAY_FEATURE_UNIT_ID;
  p_audio_output_terminal_desc->bCSourceID = USBD_AUDIO_CLOCK_SOURCE_ID;
  p_audio_output_terminal_desc->bmaControls = 0x0000U;
  p_audio_output_terminal_desc->iTerminal = 0x00U;
  *config_framework_size += (ULONG) sizeof(USBD_AUDIO_OUTPUT_TERMINAL_DESC);

  if (p_audio_control_interface->endpoints_numbers > 0U)
  {
    p_audio_control_endpoint = p_audio_control_interface->device_endpoint;

    if (p_audio_control_endpoint == UX_NULL)
    {
      return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
    }

    usb_device_framework_set_endpoint((ULONG) p_config_framework, config_framework_size,
                                      p_audio_ep_desc, p_audio_control_endpoint, device_speed);
  }

  usb_device_framework_set_interface((ULONG) p_config_framework, config_framework_size,
                                     p_audio_itf_desc, p_audio_stream_in_interface);

  stream_interface_number = p_audio_stream_in_interface->interface_numbers;
  p_audio_stream_interface_alt = p_audio_stream_in_interface->next_alt_setting;
  if (p_audio_stream_interface_alt == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  p_audio_stream_interface_alt->interface_numbers = stream_interface_number;

  usb_device_framework_set_interface((ULONG) p_config_framework, config_framework_size,
                                     p_audio_itf_desc, p_audio_stream_interface_alt);
  interface_index++;

  p_audio_as_desc = (USBD_AUDIO_AS_CS_IF_DESC *) ((ULONG) p_config_framework + *config_framework_size);
  p_audio_as_desc->bLength = (UCHAR) sizeof(USBD_AUDIO_AS_CS_IF_DESC);
  p_audio_as_desc->bDescriptorType = 0x24U;
  p_audio_as_desc->bDescriptorSubtype = 0x01U;
  p_audio_as_desc->bTerminalLink = USBD_AUDIO_CAPTURE_TERMINAL_OUTPUT_ID;
  p_audio_as_desc->bmControls = 0x00U;
  p_audio_as_desc->bFormatType = 0x01U;
  p_audio_as_desc->bmFormats = 0x00000001UL;
  p_audio_as_desc->bNrChannels = USBD_AUDIO_CHANNEL_COUNT;
  p_audio_as_desc->bmChannelConfig = USBD_AUDIO_CHANNEL_MAP;
  p_audio_as_desc->iChannelNames = 0x00U;
  *config_framework_size += (ULONG) sizeof(USBD_AUDIO_AS_CS_IF_DESC);

  p_audio_format_desc = (USBD_AUDIO_FORMAT_TYPE_DESC *) ((ULONG) p_config_framework + *config_framework_size);
  p_audio_format_desc->bLength = (UCHAR) sizeof(USBD_AUDIO_FORMAT_TYPE_DESC);
  p_audio_format_desc->bDescriptorType = 0x24U;
  p_audio_format_desc->bDescriptorSubtype = 0x02U;
  p_audio_format_desc->bFormatType = 0x01U;
  p_audio_format_desc->bSubslotSize = USBD_AUDIO_RES_BYTE;
  p_audio_format_desc->bBitResolution = USBD_AUDIO_RES_BIT;
  *config_framework_size += (ULONG) sizeof(USBD_AUDIO_FORMAT_TYPE_DESC);

  p_audio_endpoint = (USB_DEVICE_ENDPOINT_HANDLE *) p_audio_stream_interface_alt->device_endpoint;
  if (p_audio_endpoint == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  usb_device_framework_set_endpoint((ULONG) p_config_framework, config_framework_size,
                                    p_audio_ep_desc, p_audio_endpoint, device_speed);

  p_audio_cs_ep_desc = (USBD_AUDIO_CS_EP_DESC *) ((ULONG) p_config_framework + *config_framework_size);
  p_audio_cs_ep_desc->bLength = (UCHAR) sizeof(USBD_AUDIO_CS_EP_DESC);
  p_audio_cs_ep_desc->bDescriptorType = 0x25U;
  p_audio_cs_ep_desc->bDescriptorSubtype = 0x01U;
  p_audio_cs_ep_desc->bmAttributes = 0x00U;
  p_audio_cs_ep_desc->bmControls = 0x00U;
  p_audio_cs_ep_desc->bLockDelayUnits = 0x00U;
  p_audio_cs_ep_desc->wLockDelay = 0x0000U;
  *config_framework_size += (ULONG) sizeof(USBD_AUDIO_CS_EP_DESC);

  usb_device_framework_set_interface((ULONG) p_config_framework, config_framework_size,
                                     p_audio_itf_desc, p_audio_stream_out_interface);

  stream_interface_number = p_audio_stream_out_interface->interface_numbers;
  p_audio_stream_interface_alt = p_audio_stream_out_interface->next_alt_setting;
  if (p_audio_stream_interface_alt == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  p_audio_stream_interface_alt->interface_numbers = stream_interface_number;

  usb_device_framework_set_interface((ULONG) p_config_framework, config_framework_size,
                                     p_audio_itf_desc, p_audio_stream_interface_alt);
  interface_index++;

  p_audio_as_desc = (USBD_AUDIO_AS_CS_IF_DESC *) ((ULONG) p_config_framework + *config_framework_size);
  p_audio_as_desc->bLength = (UCHAR) sizeof(USBD_AUDIO_AS_CS_IF_DESC);
  p_audio_as_desc->bDescriptorType = 0x24U;
  p_audio_as_desc->bDescriptorSubtype = 0x01U;
  p_audio_as_desc->bTerminalLink = USBD_AUDIO_PLAY_TERMINAL_INPUT_ID;
  p_audio_as_desc->bmControls = 0x00U;
  p_audio_as_desc->bFormatType = 0x01U;
  p_audio_as_desc->bmFormats = 0x00000001UL;
  p_audio_as_desc->bNrChannels = USBD_AUDIO_CHANNEL_COUNT;
  p_audio_as_desc->bmChannelConfig = USBD_AUDIO_CHANNEL_MAP;
  p_audio_as_desc->iChannelNames = 0x00U;
  *config_framework_size += (ULONG) sizeof(USBD_AUDIO_AS_CS_IF_DESC);

  p_audio_format_desc = (USBD_AUDIO_FORMAT_TYPE_DESC *) ((ULONG) p_config_framework + *config_framework_size);
  p_audio_format_desc->bLength = (UCHAR) sizeof(USBD_AUDIO_FORMAT_TYPE_DESC);
  p_audio_format_desc->bDescriptorType = 0x24U;
  p_audio_format_desc->bDescriptorSubtype = 0x02U;
  p_audio_format_desc->bFormatType = 0x01U;
  p_audio_format_desc->bSubslotSize = USBD_AUDIO_RES_BYTE;
  p_audio_format_desc->bBitResolution = USBD_AUDIO_RES_BIT;
  *config_framework_size += (ULONG) sizeof(USBD_AUDIO_FORMAT_TYPE_DESC);

  p_audio_endpoint = (USB_DEVICE_ENDPOINT_HANDLE *) p_audio_stream_interface_alt->device_endpoint;
  if (p_audio_endpoint == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  usb_device_framework_set_endpoint((ULONG) p_config_framework, config_framework_size,
                                    p_audio_ep_desc, p_audio_endpoint, device_speed);

  p_audio_cs_ep_desc = (USBD_AUDIO_CS_EP_DESC *) ((ULONG) p_config_framework + *config_framework_size);
  p_audio_cs_ep_desc->bLength = (UCHAR) sizeof(USBD_AUDIO_CS_EP_DESC);
  p_audio_cs_ep_desc->bDescriptorType = 0x25U;
  p_audio_cs_ep_desc->bDescriptorSubtype = 0x01U;
  p_audio_cs_ep_desc->bmAttributes = 0x00U;
  p_audio_cs_ep_desc->bmControls = 0x00U;
  p_audio_cs_ep_desc->bLockDelayUnits = 0x00U;
  p_audio_cs_ep_desc->wLockDelay = 0x0000U;
  *config_framework_size += (ULONG) sizeof(USBD_AUDIO_CS_EP_DESC);

  if (p_audio_stream_interface_alt->endpoints_numbers > 1U)
  {
    p_audio_endpoint = &p_audio_stream_interface_alt->device_endpoint[1];
    usb_device_framework_set_endpoint((ULONG) p_config_framework, config_framework_size,
                                      p_audio_ep_desc, p_audio_endpoint, device_speed);

  }

  ((USBD_CONFIGURATION_DESC *)p_config_framework)->wTotalLength = *config_framework_size;
  ((USBD_CONFIGURATION_DESC *)p_config_framework)->bNumInterfaces += interface_index;

  return USBD_DESCRIPTOR_SUCCESS;
}
#endif /* USBD_AUDIO_CLASS_ACTIVATED */

#if USBD_PRINTER_CLASS_ACTIVATED == 1U
/**
  * @brief  usb_device_printer_framework_builder
  *         Configure and Append the PRINTER Descriptor
  * @retval none
  */
static UINT usb_device_printer_framework_builder(USB_DEVICE_INTERFACE_HANDLE *p_interface,
                                                 ULONG p_config_framework,
                                                 ULONG *config_framework_size,
                                                 UCHAR device_speed)
{
  UCHAR                       interface_index = 0U;
  USBD_INTERFACE_DESC         *p_printer_itf_desc = UX_NULL;
  USBD_ENDPOINT_DESC          *p_printer_ep_desc = UX_NULL;
  USB_DEVICE_INTERFACE_HANDLE *p_printer_interface = UX_NULL;
  USB_DEVICE_ENDPOINT_HANDLE  *p_printer_in_endpoint = UX_NULL;
  USB_DEVICE_ENDPOINT_HANDLE  *p_printer_out_endpoint = UX_NULL;

  p_printer_interface = (USB_DEVICE_INTERFACE_HANDLE *)p_interface;

  if (p_printer_interface == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  usb_device_framework_set_interface((ULONG) p_config_framework, config_framework_size,
                                     p_printer_itf_desc, p_printer_interface);

  interface_index++;

  p_printer_in_endpoint = (USB_DEVICE_ENDPOINT_HANDLE *)p_printer_interface->device_endpoint;

  if (p_printer_in_endpoint == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  usb_device_framework_set_endpoint((ULONG) p_config_framework, config_framework_size,
                                    p_printer_ep_desc, p_printer_in_endpoint, device_speed);

  if (p_printer_interface->endpoints_numbers > 1U)
  {
    p_printer_in_endpoint->next_endpoint = &p_printer_interface->device_endpoint[1];
    p_printer_out_endpoint = (USB_DEVICE_ENDPOINT_HANDLE *)p_printer_in_endpoint->next_endpoint;

    if (p_printer_out_endpoint == UX_NULL)
    {
      return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
    }

    usb_device_framework_set_endpoint((ULONG) p_config_framework, config_framework_size,
                                      p_printer_ep_desc, p_printer_out_endpoint, device_speed);
  }

  ((USBD_CONFIGURATION_DESC *)p_config_framework)->wTotalLength = *config_framework_size;
  ((USBD_CONFIGURATION_DESC *)p_config_framework)->bNumInterfaces += interface_index;

  return USBD_DESCRIPTOR_SUCCESS;
}
#endif /* USBD_PRINTER_CLASS_ACTIVATED */

#if USBD_CCID_CLASS_ACTIVATED == 1U
/**
  * @brief  usb_device_ccid_framework_builder
  *         Configure and Append the CCID Descriptor
  * @retval none
  */
static UINT usb_device_ccid_framework_builder(USB_DEVICE_INTERFACE_HANDLE *p_interface,
                                              ULONG p_config_framework,
                                              ULONG *config_framework_size,
                                              UCHAR device_speed)
{
  UCHAR interface_index = 0U;
  USBD_INTERFACE_DESC *p_ccid_itf_desc = UX_NULL;
  USBD_ENDPOINT_DESC *p_ccid_ep_desc = UX_NULL;
  USBD_CCID_DESC *p_ccid_desc = UX_NULL;
  USB_DEVICE_INTERFACE_HANDLE *p_ccid_interface = UX_NULL;
  USB_DEVICE_ENDPOINT_HANDLE *p_ccid_in_endpoint = UX_NULL;
  USB_DEVICE_ENDPOINT_HANDLE *p_ccid_out_endpoint = UX_NULL;
  USB_DEVICE_ENDPOINT_HANDLE *p_ccid_ctl_endpoint = UX_NULL;

  p_ccid_interface = (USB_DEVICE_INTERFACE_HANDLE *) p_interface;

  if (p_ccid_interface == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  usb_device_framework_set_interface((ULONG) p_config_framework, config_framework_size,
                                     p_ccid_itf_desc, p_ccid_interface);

  interface_index++;

  p_ccid_desc = (USBD_CCID_DESC *) ((ULONG) p_config_framework + *config_framework_size);
  p_ccid_desc->bLength = (UCHAR) sizeof(USBD_CCID_DESC);
  p_ccid_desc->bDescriptorType = 0x21U;
  p_ccid_desc->bcdCCID = UX_DEVICE_CLASS_CCID_BCD_CCID_1_10;
  p_ccid_desc->bMaxSlotIndex = USBD_CCID_MAX_SLOT_INDEX;
  p_ccid_desc->bVoltageSupport = USBD_CCID_VOLTAGE_SUPPLY;
  p_ccid_desc->dwProtocols = USBD_CCID_PROTOCOL;
  p_ccid_desc->dwDefaultClock = USBD_CCID_DEFAULT_CLOCK_FREQ;
  p_ccid_desc->dwMaximumClock = USBD_CCID_MAX_CLOCK_FREQ;
  p_ccid_desc->bNumClockSupported = USBD_CCID_N_CLOCKS;
  p_ccid_desc->dwDataRate = USBD_CCID_DEFAULT_DATA_RATE;
  p_ccid_desc->dwMaxDataRate = USBD_CCID_MAX_DATA_RATE;
  p_ccid_desc->bNumDataRatesSupported = USBD_CCID_N_DATA_RATES;
  p_ccid_desc->dwMaxIFSD = USBD_CCID_MAX_IFSD;
  p_ccid_desc->dwSynchProtocols = USBD_CCID_SYNCH_PROTOCOLS;
  p_ccid_desc->dwMechanical = USBD_CCID_MECHANICAL;
  p_ccid_desc->dwFeatures = USBD_CCID_FEATURES;
  p_ccid_desc->dwMaxCCIDMessageLength = USBD_CCID_MAX_MESSAGE_LENGTH;
  p_ccid_desc->bClassGetResponse = USBD_CCID_CLASS_GET_RESPONSE;
  p_ccid_desc->bClassEnvelope = USBD_CCID_CLASS_ENVELOPE;
  p_ccid_desc->wLcdLayout = USBD_CCID_LCD_LAYOUT;
  p_ccid_desc->bPINSupport = USBD_CCID_PIN_SUPPORT;
  p_ccid_desc->bMaxCCIDBusySlots = USBD_CCID_MAX_BUSY_SLOTS;
  *config_framework_size += (ULONG) sizeof(USBD_CCID_DESC);

  p_ccid_in_endpoint = (USB_DEVICE_ENDPOINT_HANDLE *)p_ccid_interface->device_endpoint;

  if (p_ccid_in_endpoint == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

    if (p_ccid_interface->endpoints_numbers < 3U)
    {
      return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
    }

  usb_device_framework_set_endpoint((ULONG) p_config_framework, config_framework_size,
                                    p_ccid_ep_desc, p_ccid_in_endpoint, device_speed);

  p_ccid_in_endpoint->next_endpoint = &p_ccid_interface->device_endpoint[1];
  p_ccid_out_endpoint = (USB_DEVICE_ENDPOINT_HANDLE *) p_ccid_in_endpoint->next_endpoint;

  if (p_ccid_out_endpoint == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  usb_device_framework_set_endpoint((ULONG) p_config_framework, config_framework_size,
                                    p_ccid_ep_desc, p_ccid_out_endpoint, device_speed);

  p_ccid_out_endpoint->next_endpoint = &p_ccid_interface->device_endpoint[2];
  p_ccid_ctl_endpoint = (USB_DEVICE_ENDPOINT_HANDLE *) p_ccid_out_endpoint->next_endpoint;

  if (p_ccid_ctl_endpoint == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  usb_device_framework_set_endpoint((ULONG) p_config_framework, config_framework_size,
                                    p_ccid_ep_desc, p_ccid_ctl_endpoint, device_speed);

  ((USBD_CONFIGURATION_DESC *)p_config_framework)->wTotalLength = *config_framework_size;
  ((USBD_CONFIGURATION_DESC *)p_config_framework)->bNumInterfaces += interface_index;

  return USBD_DESCRIPTOR_SUCCESS;
}
#endif /* USBD_CCID_CLASS_ACTIVATED */

#if USBD_HID_CLASS_ACTIVATED == 1U
/**
  * @brief  usb_device_hid_framework_builder
  *         Configure and Append the HID Descriptor
  * @retval none
  */
static UINT usb_device_hid_framework_builder(USB_DEVICE_INTERFACE_HANDLE *p_interface,
                                             ULONG p_config_framework,
                                             ULONG *config_framework_size, UCHAR device_speed)
{
  UCHAR                 interface_index = 0U;
  USBD_HID_DESC         *p_hid_desc = UX_NULL;
  USBD_INTERFACE_DESC   *p_hid_itf_desc = UX_NULL;
  USBD_ENDPOINT_DESC    *p_hid_ep_desc = UX_NULL;

  USB_DEVICE_INTERFACE_HANDLE   *p_hid_interface = UX_NULL;
  USB_DEVICE_ENDPOINT_HANDLE    *p_hid_in_endpoint = UX_NULL;
  USB_DEVICE_ENDPOINT_HANDLE    *p_hid_out_endpoint = UX_NULL;

  /* Get USB HID Interface */
  p_hid_interface = (USB_DEVICE_INTERFACE_HANDLE *)p_interface;

  if (p_hid_interface == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  /* Append HID Interface descriptor to Configuration descriptor */
  usb_device_framework_set_interface((ULONG) p_config_framework, config_framework_size,
                                     p_hid_itf_desc, p_hid_interface);

  /* Increment interface index */
  interface_index++;

  /* Append HID Functional descriptor to Configuration descriptor */
  p_hid_desc = (USBD_HID_DESC *)((ULONG)p_config_framework + *config_framework_size);
  p_hid_desc->bLength            = (UCHAR)sizeof(USBD_HID_DESC);
  p_hid_desc->bDescriptorType    = UX_DEVICE_CLASS_HID_DESCRIPTOR_HID;
  p_hid_desc->bcdHID             = USBD_HID_BCD;
  p_hid_desc->bCountryCode       = USBD_HID_BCOUNTRY_CODE;
  p_hid_desc->bNumDescriptors    = USBD_HID_NUM_DESCRIPTORS;
  p_hid_desc->bHIDDescriptorType = USBD_HID_DESCRIPTOR_TYPE;
  p_hid_desc->wDescriptorLength  = p_hid_interface->other_parameter;
  *config_framework_size += (ULONG)sizeof(USBD_HID_DESC);

  /* Get HID IN Endpoint */
  p_hid_in_endpoint = (USB_DEVICE_ENDPOINT_HANDLE *)p_hid_interface->device_endpoint;

  if (p_hid_in_endpoint == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  /* Append Endpoint Data IN descriptor to Configuration descriptor */
  usb_device_framework_set_endpoint((ULONG) p_config_framework, config_framework_size,
                                    p_hid_ep_desc, p_hid_in_endpoint, device_speed);

  if (p_hid_interface->endpoints_numbers > 1U)
  {
    p_hid_in_endpoint->next_endpoint = &p_hid_interface->device_endpoint[1];

    /* Get HID OUT Endpoint */
    p_hid_out_endpoint = (USB_DEVICE_ENDPOINT_HANDLE *)p_hid_in_endpoint->next_endpoint;

    if (p_hid_out_endpoint != UX_NULL)
    {
      /* Append Endpoint Data OUT descriptor to Configuration descriptor */
      usb_device_framework_set_endpoint((ULONG) p_config_framework, config_framework_size,
                                        p_hid_ep_desc, p_hid_out_endpoint, device_speed);
    }
  }

  /* Update Config Descriptor */
  ((USBD_CONFIGURATION_DESC *)p_config_framework)->wTotalLength = *config_framework_size;
  ((USBD_CONFIGURATION_DESC *)p_config_framework)->bNumInterfaces += interface_index;

  return USBD_DESCRIPTOR_SUCCESS;
}
#endif /* USBD_HID_CLASS_ACTIVATED */

#if USBD_CDC_ACM_CLASS_ACTIVATED == 1U
/**
  * @brief  usb_device_cdc_acm_framework_builder
  *         Configure and Append the CDC Descriptor
  * @retval none
  */
static UINT usb_device_cdc_acm_framework_builder(USB_DEVICE_INTERFACE_HANDLE *p_interface,
                                                 ULONG p_config_framework,
                                                 ULONG *config_framework_size, UCHAR device_speed)
{
  UCHAR                             interface_index = 0U;
  USBD_INTERFACE_DESC               *p_cdc_itf_desc = UX_NULL;
  USBD_ENDPOINT_DESC                *p_cdc_ep_desc = UX_NULL;

  USBD_CDC_HEADER_DESC              *pHeadDesc = UX_NULL;
  USBD_CDC_CALL_MANAGEMENT_DESC     *pCallMgmDesc = UX_NULL;
  USBD_CDC_ACM_DESC                 *pACMDesc = UX_NULL;
  USBD_CDC_UNION_DESC               *pUnionDesc = UX_NULL;

  USB_DEVICE_INTERFACE_HANDLE       *cdc_ctl_interface = UX_NULL;
  USB_DEVICE_INTERFACE_HANDLE       *cdc_data_interface = UX_NULL;
  USB_DEVICE_ENDPOINT_HANDLE        *p_cdc_ctl_endpoint = UX_NULL;
  USB_DEVICE_ENDPOINT_HANDLE        *p_cdc_in_endpoint = UX_NULL;
  USB_DEVICE_ENDPOINT_HANDLE        *p_cdc_out_endpoint = UX_NULL;
#if USBD_COMPOSITE_USE_IAD == 1
  USBD_IAD_DESC                     *pIadDesc;
#endif /* USBD_COMPOSITE_USE_IAD == 1 */

#if USBD_COMPOSITE_USE_IAD == 1
  pIadDesc = (USBD_IAD_DESC *)((ULONG)p_config_framework + *config_framework_size);
  pIadDesc->bLength = (uint8_t)sizeof(USBD_IAD_DESC);
  pIadDesc->bDescriptorType = USB_DESC_TYPE_IAD; /* IAD descriptor */
  pIadDesc->bFirstInterface = p_interface->interface_numbers;
  pIadDesc->bInterfaceCount = 2U;    /* 2 interfaces */
  pIadDesc->bFunctionClass = 0x02U;
  pIadDesc->bFunctionSubClass = 0x02U;
  pIadDesc->bFunctionProtocol = 0x01U;
  pIadDesc->iFunction = 0; /* String Index */
  *config_framework_size += (uint32_t)sizeof(USBD_IAD_DESC);
#endif /* USBD_COMPOSITE_USE_IAD == 1 */

  /* Get USB CONTROL CDC Interface */
  cdc_ctl_interface = (USB_DEVICE_INTERFACE_HANDLE *)p_interface;

  if (cdc_ctl_interface == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  /* Append CDC Interface descriptor to Configuration descriptor */
  usb_device_framework_set_interface((ULONG) p_config_framework, config_framework_size,
                                     p_cdc_itf_desc, cdc_ctl_interface);

  /* Increment interface index */
  interface_index++;

  /* Control interface headers */
  pHeadDesc = ((USBD_CDC_HEADER_DESC *)((uint32_t)p_config_framework + *config_framework_size));
  /* Header Functional Descriptor*/
  pHeadDesc->bLength = 0x05U;
  pHeadDesc->bDescriptorType = 0x24U;
  pHeadDesc->bDescriptorSubtype = 0x00U;
  pHeadDesc->bcdCDC = 0x0110;
  *config_framework_size += (uint32_t)sizeof(USBD_CDC_HEADER_DESC);

  /* Call Management Functional Descriptor*/
  pCallMgmDesc = ((USBD_CDC_CALL_MANAGEMENT_DESC *)((uint32_t)p_config_framework + *config_framework_size));
  pCallMgmDesc->bLength = 0x05U;
  pCallMgmDesc->bDescriptorType = 0x24U;
  pCallMgmDesc->bDescriptorSubtype = 0x01U;
  pCallMgmDesc->bmCapabilities = 0x00U;
  pCallMgmDesc->bDataInterface = 0x00U;
  *config_framework_size += (uint32_t)sizeof(USBD_CDC_CALL_MANAGEMENT_DESC);

  /* ACM Functional Descriptor*/
  pACMDesc = ((USBD_CDC_ACM_DESC *)((uint32_t)p_config_framework + *config_framework_size));
  pACMDesc->bLength = 0x04U;
  pACMDesc->bDescriptorType = 0x24U;
  pACMDesc->bDescriptorSubtype = 0x02U;
  pACMDesc->bmCapabilities = 0x02;
  *config_framework_size += (uint32_t)sizeof(USBD_CDC_ACM_DESC);

  /* Union Functional Descriptor*/
  pUnionDesc = ((USBD_CDC_UNION_DESC *)((uint32_t)p_config_framework + *config_framework_size));
  pUnionDesc->bLength = 0x05U;
  pUnionDesc->bDescriptorType = 0x24U;
  pUnionDesc->bDescriptorSubtype = 0x06U;
  pUnionDesc->bMasterInterface = cdc_ctl_interface->interface_numbers;
  pUnionDesc->bSlaveInterface = 0x00U;
  *config_framework_size += (uint32_t)sizeof(USBD_CDC_UNION_DESC);

     /* Get CDC Notification Endpoint */
  p_cdc_ctl_endpoint = (USB_DEVICE_ENDPOINT_HANDLE *)cdc_ctl_interface->device_endpoint;

  if (p_cdc_ctl_endpoint != UX_NULL)
  {
    /* Append Endpoint Notification descriptor to Configuration descriptor */
    usb_device_framework_set_endpoint((ULONG) p_config_framework, config_framework_size,
                                      p_cdc_ep_desc, p_cdc_ctl_endpoint, device_speed);
  }

  /* Get USB DATA CDC Interface */
  cdc_data_interface = (USB_DEVICE_INTERFACE_HANDLE *)p_interface->next_interface;

  if (cdc_data_interface == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  pCallMgmDesc->bDataInterface = cdc_data_interface->interface_numbers;
  pUnionDesc->bSlaveInterface = cdc_data_interface->interface_numbers;

  /* Append CDC Interface descriptor to Configuration descriptor */
  usb_device_framework_set_interface((ULONG) p_config_framework, config_framework_size,
                                     p_cdc_itf_desc, cdc_data_interface);

  /* Increment interface index */
  interface_index++;

  /* Get CDC IN Endpoint */
  p_cdc_in_endpoint = (USB_DEVICE_ENDPOINT_HANDLE *)cdc_data_interface->device_endpoint;

  if (p_cdc_in_endpoint == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  /* Append Endpoint IN descriptor to Configuration descriptor */
  usb_device_framework_set_endpoint((ULONG) p_config_framework, config_framework_size,
                                    p_cdc_ep_desc, p_cdc_in_endpoint, device_speed);

  p_cdc_in_endpoint->next_endpoint = &cdc_data_interface->device_endpoint[1];

  /* Get CDC OUT Endpoint */
  p_cdc_out_endpoint = (USB_DEVICE_ENDPOINT_HANDLE *)p_cdc_in_endpoint->next_endpoint;

  if (p_cdc_out_endpoint == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  /* Append Endpoint Data OUT descriptor to Configuration descriptor */
  usb_device_framework_set_endpoint((ULONG) p_config_framework, config_framework_size,
                                    p_cdc_ep_desc, p_cdc_out_endpoint, device_speed);

  /* Update Config Descriptor */
  ((USBD_CONFIGURATION_DESC *)p_config_framework)->wTotalLength = *config_framework_size;
  ((USBD_CONFIGURATION_DESC *)p_config_framework)->bNumInterfaces += interface_index;

  return USBD_DESCRIPTOR_SUCCESS;
}
#endif /* USBD_CDC_ACM_CLASS_ACTIVATED */

#if USBD_MTP_CLASS_ACTIVATED == 1U
/**
  * @brief  usb_device_mtp_framework_builder
  *         Configure and Append the MTP Descriptor
  * @retval none
  */
static UINT usb_device_mtp_framework_builder(USB_DEVICE_INTERFACE_HANDLE *p_interface,
                                             ULONG p_config_framework,
                                             ULONG *config_framework_size, UCHAR device_speed)
{
  UCHAR                             interface_index = 0U;
  USBD_INTERFACE_DESC               *p_mtp_itf_desc = UX_NULL;
  USBD_ENDPOINT_DESC                *p_mtp_ep_desc = UX_NULL;

  USB_DEVICE_INTERFACE_HANDLE       *p_mtp_interface = UX_NULL;
  USB_DEVICE_ENDPOINT_HANDLE        *p_mtp_ctl_endpoint = UX_NULL;
  USB_DEVICE_ENDPOINT_HANDLE        *p_mtp_in_endpoint = UX_NULL;
  USB_DEVICE_ENDPOINT_HANDLE        *p_mtp_out_endpoint = UX_NULL;

  /* Get USB CONTROL MTP Interface */
  p_mtp_interface = (USB_DEVICE_INTERFACE_HANDLE *)p_interface;

  if (p_mtp_interface == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  /* Append MTP Interface descriptor to Configuration descriptor */
  usb_device_framework_set_interface((ULONG) p_config_framework, config_framework_size,
                                     p_mtp_itf_desc, p_mtp_interface);

  /* Increment interface index */
  interface_index++;

  if (p_mtp_interface->device_endpoint == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  /* Get MTP IN Endpoint */
  p_mtp_in_endpoint = (USB_DEVICE_ENDPOINT_HANDLE *)p_mtp_interface->device_endpoint;

  if (p_mtp_in_endpoint != UX_NULL)
  {
    /* Append Endpoint IN descriptor to Configuration descriptor */
    usb_device_framework_set_endpoint((ULONG) p_config_framework, config_framework_size,
                                      p_mtp_ep_desc, p_mtp_in_endpoint, device_speed);
  }

  p_mtp_in_endpoint->next_endpoint = &p_mtp_interface->device_endpoint[1];

  /* Get MTP OUT Endpoint */
  p_mtp_out_endpoint = (USB_DEVICE_ENDPOINT_HANDLE *)p_mtp_in_endpoint->next_endpoint;

  if (p_mtp_out_endpoint == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  /* Append Endpoint OUT descriptor to Configuration descriptor */
  usb_device_framework_set_endpoint((ULONG) p_config_framework, config_framework_size,
                                    p_mtp_ep_desc, p_mtp_out_endpoint, device_speed);

  p_mtp_out_endpoint->next_endpoint = &p_mtp_interface->device_endpoint[2];

  /* Get MTP CTL Endpoint */
  p_mtp_ctl_endpoint = (USB_DEVICE_ENDPOINT_HANDLE *)p_mtp_out_endpoint->next_endpoint;

  if (p_mtp_ctl_endpoint == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  /* Append Endpoint CTL descriptor to Configuration descriptor */
  usb_device_framework_set_endpoint((ULONG) p_config_framework, config_framework_size,
                                    p_mtp_ep_desc, p_mtp_ctl_endpoint, device_speed);

  /* Update Config Descriptor */
  ((USBD_CONFIGURATION_DESC *)p_config_framework)->wTotalLength = *config_framework_size;
  ((USBD_CONFIGURATION_DESC *)p_config_framework)->bNumInterfaces += interface_index;

  return USBD_DESCRIPTOR_SUCCESS;
}
#endif /* USBD_MTP_CLASS_ACTIVATED */

#if USBD_VIDEO_CLASS_ACTIVATED == 1U
/**
  * @brief  usb_device_video_framework_builder
  *         Configure and Append the VIDEO Descriptor
  * @retval none
  */
static UINT usb_device_video_framework_builder(USB_DEVICE_INTERFACE_HANDLE *p_interface,
                                             ULONG p_config_framework,
                                             ULONG *config_framework_size, UCHAR device_speed)
{
  UCHAR                                 interface_index = 0U;
  UCHAR                                 vs_interface_number = 0U;
  USBD_INTERFACE_DESC                   *p_video_itf_desc = UX_NULL;
  USBD_ENDPOINT_DESC                    *p_video_ep_desc = UX_NULL;

  USBD_VIDEO_DESC                       *p_video_desc = UX_NULL;
  USBD_VIDEO_INPUT_TERMINAL_DESC        *pVideoITDesc = UX_NULL;
  USBD_VIDEO_OUTPUT_TERMINAL_DESC       *pVideoOTDesc = UX_NULL;
  USBD_VIDEO_VS_HEADER_DESC             *pVideoVSHeaderDesc = UX_NULL;
  USBD_VIDEO_PAYLOAD_FORMAT_DESC        *pVideoPayloadFormatDesc = UX_NULL;
  USBD_VIDEO_FRAME_DESC                 *pVideoFrameDesc = UX_NULL;
  USBD_VIDEO_PROCESSING_UNIT_DESCRIPTOR *pVideoPUDesc = UX_NULL;

  USB_DEVICE_INTERFACE_HANDLE           *p_video_interface = UX_NULL;
  USB_DEVICE_INTERFACE_HANDLE           *p_video_streaming_interface = UX_NULL;
  USB_DEVICE_ENDPOINT_HANDLE            *p_video_endpoint = UX_NULL;

#if USBD_COMPOSITE_USE_IAD == 1
  USBD_IAD_DESC                     *pIadDesc;
#endif /* USBD_COMPOSITE_USE_IAD == 1 */

#if USBD_COMPOSITE_USE_IAD == 1
  pIadDesc = (USBD_IAD_DESC *)((ULONG)p_config_framework + *config_framework_size);
  pIadDesc->bLength = (uint8_t)sizeof(USBD_IAD_DESC);
  pIadDesc->bDescriptorType = USB_DESC_TYPE_IAD; /* IAD descriptor */
  pIadDesc->bFirstInterface = p_interface->interface_numbers;
  pIadDesc->bInterfaceCount = 2U;    /* 2 interfaces */
  pIadDesc->bFunctionClass = UX_DEVICE_CLASS_VIDEO_CC_VIDEO;
  pIadDesc->bFunctionSubClass = UX_DEVICE_CLASS_VIDEO_SC_INTERFACE_COLLECTION;
  pIadDesc->bFunctionProtocol = UX_DEVICE_CLASS_VIDEO_PC_PROTOCOL_UNDEFINED;
#if USBD_UVC_USE_FRAME_BASE_H264 ==1
  pIadDesc->bFunctionProtocol = UX_DEVICE_CLASS_VIDEO_PC_PROTOCOL_15;
#endif  /* USBD_UVC_USE_FRAME_BASE_H264 == 1 */
  pIadDesc->iFunction = 0U; /* String Index */
  *config_framework_size += (uint32_t)sizeof(USBD_IAD_DESC);
#endif /* USBD_COMPOSITE_USE_IAD == 1 */

  /* Get USB CONTROL VIDEO Interface */
  p_video_interface = (USB_DEVICE_INTERFACE_HANDLE *)p_interface;

  if (p_video_interface == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  /* Append VIDEO Interface descriptor to Configuration descriptor */
  usb_device_framework_set_interface((ULONG) p_config_framework, config_framework_size,
                                     p_video_itf_desc, p_video_interface);

  /* Increment interface index */
  interface_index++;

  /* Append Class-specific VC Interface Descriptor to Configuration descriptor*/
  p_video_desc = (USBD_VIDEO_DESC *)((ULONG)p_config_framework + *config_framework_size);
  p_video_desc->bLength = (uint8_t)sizeof(USBD_VIDEO_DESC);
  p_video_desc->bDescriptorType = UX_DEVICE_CLASS_VIDEO_CS_INTERFACE;
  p_video_desc->bDescriptorSubtype = 0x01U;
  p_video_desc->bcdUVC = 0x0110U;
  p_video_desc->wTotalLength = VC_HEADER_SIZE;

#if (USBD_UVC_USE_FRAME_BASE_H264 == 1U)
  p_video_desc->dwClockFrequency = 0U;
#else
  p_video_desc->dwClockFrequency = 0x02DC6C00U;
#endif /* USBD_UVC_USE_FRAME_BASE_H264 */

  p_video_desc->bInCollection = 0x01U;

  /* Standard VS (Video Streaming) Interface Descriptor */
  /* Interface 1, Alternate Setting 0 = Zero Bandwidth */
  p_video_streaming_interface = (USB_DEVICE_INTERFACE_HANDLE *)p_video_interface->next_interface;

  if (p_video_streaming_interface == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  p_video_desc->aInterfaceNr = p_video_streaming_interface->interface_numbers;
  *config_framework_size += (uint32_t)sizeof(USBD_VIDEO_DESC);

  /* Append Input Terminal Descriptor to Configuration descriptor */
  pVideoITDesc = (USBD_VIDEO_INPUT_TERMINAL_DESC *)((ULONG)p_config_framework + *config_framework_size);
  pVideoITDesc->bLength = (uint8_t)sizeof(USBD_VIDEO_INPUT_TERMINAL_DESC);
  pVideoITDesc->bDescriptorType = UX_DEVICE_CLASS_VIDEO_CS_INTERFACE;
  pVideoITDesc->bDescriptorSubtype = 0x02U;
  pVideoITDesc->bTerminalID = 0x01U;
  pVideoITDesc->wTerminalType = 0x0200U;
#if (USBD_UVC_USE_CAMERA == 1U)
  pVideoITDesc->wTerminalType = 0x0201U; /* CAMERA */
  pVideoITDesc->bAssocTerminal = 0x00U;
  pVideoITDesc->iTerminal =  0x00U;
  pVideoITDesc->wObjectiveFocalLengthMin = 0x0000; /* wObjectiveFocalLengthMin */
  pVideoITDesc->wObjectiveFocalLengthMax = 0x0000; /* wObjectiveFocalLengthMax */
  pVideoITDesc->wOcularFocalLength = 0x0000;       /* wOcularFocalLength       */
  pVideoITDesc->bControlSize = 0x03;               /* bControlSize             */
  pVideoITDesc->bmControls[0] = 0x00U;             /* bmControls               */
  pVideoITDesc->bmControls[1] = 0x00U;
  pVideoITDesc->bmControls[2] = 0x00U;
#else
  pVideoITDesc->wTerminalType = 0x0200U;  /* Custom */
  pVideoITDesc->bAssocTerminal = 0x00U;
  pVideoITDesc->iTerminal = 0x00U;
#endif /* USBD_UVC_USE_CAMERA */

  *config_framework_size += (uint32_t)sizeof(USBD_VIDEO_INPUT_TERMINAL_DESC);

  /* Append Output Terminal Descriptor to Configuration descriptor */
  pVideoOTDesc = (USBD_VIDEO_OUTPUT_TERMINAL_DESC *)((ULONG)p_config_framework + *config_framework_size);
  pVideoOTDesc->bLength = (uint8_t)sizeof(USBD_VIDEO_OUTPUT_TERMINAL_DESC);
  pVideoOTDesc->bDescriptorType = UX_DEVICE_CLASS_VIDEO_CS_INTERFACE;
  pVideoOTDesc->bDescriptorSubtype = 0x03U;
  pVideoOTDesc->bTerminalID = 0x02U;
  pVideoOTDesc->wTerminalType = 0x0101U;
  pVideoOTDesc->bAssocTerminal = 0x00U;
  pVideoOTDesc->bSourceID = 0x01U;
  pVideoOTDesc->iTerminal = 0x00U;
  *config_framework_size += (uint32_t)sizeof(USBD_VIDEO_OUTPUT_TERMINAL_DESC);

/* Append Processing Unit Descriptor to Configuration descriptor */
  pVideoPUDesc = ((USBD_VIDEO_PROCESSING_UNIT_DESCRIPTOR *)((ULONG)p_config_framework + *config_framework_size));
  pVideoPUDesc->bLength = 0x0DU;
  pVideoPUDesc->bDescriptorType = 0x24U;
  pVideoPUDesc->bDescriptorSubtype = 0x05U;
  pVideoPUDesc->bUnitID = 0x02U;
  pVideoPUDesc->bSourceID = 0x01U;
  pVideoPUDesc->wMaxMultiplier = 0x0400U;
  pVideoPUDesc->bControlSize =0x03U;
  pVideoPUDesc->bmControls[0] = 0x03U;
  pVideoPUDesc->bmControls[1] = 0x00U;
  pVideoPUDesc->bmControls[2] = 0x00U;
  pVideoPUDesc->iProcessing = 0x00U;
  pVideoPUDesc->bmVideoStandards = 0x1FU;
  *config_framework_size += (uint32_t)sizeof(USBD_VIDEO_PROCESSING_UNIT_DESCRIPTOR);

  /* Standard VS (Video Streaming) Interface Descriptor */
  /* Interface 1, Alternate Setting 0 = Zero Bandwidth */
  p_video_streaming_interface = (USB_DEVICE_INTERFACE_HANDLE *)p_video_interface->next_interface;

  if (p_video_streaming_interface == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  /* Append Video Streaming Interface descriptor to Configuration descriptor */
  usb_device_framework_set_interface((ULONG)p_config_framework, config_framework_size,
                                     p_video_itf_desc, p_video_streaming_interface);

  /* Increment interface index */
  interface_index++;

  /* Append Class-specific VS Header Descriptor (Input) to Configuration descriptor */
  pVideoVSHeaderDesc = (USBD_VIDEO_VS_HEADER_DESC *)((ULONG)p_config_framework + *config_framework_size);
  pVideoVSHeaderDesc->bLength = (uint8_t)sizeof(USBD_VIDEO_VS_HEADER_DESC);
  pVideoVSHeaderDesc->bDescriptorType = UX_DEVICE_CLASS_VIDEO_CS_INTERFACE;
  pVideoVSHeaderDesc->bDescriptorSubtype = UX_DEVICE_CLASS_VIDEO_VC_HEADER;
  pVideoVSHeaderDesc->bNumFormats = 0x01U;
  pVideoVSHeaderDesc->wTotalLength = VS_HEADER_SIZE;
  pVideoVSHeaderDesc->bEndpointAddress = USBD_VIDEO_EPIN_ADDR;
  pVideoVSHeaderDesc->bmInfo = 0x00U;
  pVideoVSHeaderDesc->bTerminalLink = 0x02U;
  pVideoVSHeaderDesc->bStillCaptureMethod = 0x00U;
  pVideoVSHeaderDesc->bTriggerSupport = 0x00U;
  pVideoVSHeaderDesc->bTriggerUsage = 0x00U;
  pVideoVSHeaderDesc->bControlSize = 0x01U;
  pVideoVSHeaderDesc->bmaControls = 0x00U;
  *config_framework_size += (uint32_t)sizeof(USBD_VIDEO_VS_HEADER_DESC);

  /* Append Payload Format Descriptor to Configuration descriptor */
  pVideoPayloadFormatDesc = (USBD_VIDEO_PAYLOAD_FORMAT_DESC *)((ULONG)p_config_framework + *config_framework_size);
  pVideoPayloadFormatDesc->bLength = (uint8_t)sizeof(USBD_VIDEO_PAYLOAD_FORMAT_DESC);
  pVideoPayloadFormatDesc->bDescriptorType = UX_DEVICE_CLASS_VIDEO_CS_INTERFACE;

#if (USBD_UVC_USE_H264 == 1U)
  pVideoPayloadFormatDesc->bDescriptorSubType = UX_DEVICE_CLASS_VIDEO_VS_FORMAT_H264;
  pVideoPayloadFormatDesc->bFormatIndex = 0x1U;
  pVideoPayloadFormatDesc->bNumFrameDescriptors = 0x1U;
  pVideoPayloadFormatDesc->bDefaultFrameIndex = 0x1U;
  pVideoPayloadFormatDesc->bMaxCodecConfigDelay = 0x0U;
  pVideoPayloadFormatDesc->bmSupportedSliceModes = 0x0U;
  pVideoPayloadFormatDesc->bmSupportedSyncFrameTypes = 0x0U;
  pVideoPayloadFormatDesc->bResolutionScaling = 0x0U;
  pVideoPayloadFormatDesc->Reserved = 0x0U;
  pVideoPayloadFormatDesc->bmSupportedRateControlModes = 0x0U;
  pVideoPayloadFormatDesc->wMaxMBperSecOneResNoScal = ((UVC_FRAME_WIDTH / 16) * (UVC_FRAME_HEIGHT / 16) * UVC_CAM_FPS_HS) / 1000U;
  pVideoPayloadFormatDesc->wMaxMBperSecTwoResNoScal = 0x0000;
  pVideoPayloadFormatDesc->wMaxMBperSecThreeResNoScal = 0x0000;
  pVideoPayloadFormatDesc->wMaxMBperSecFourResNoScal = 0x0000;
  pVideoPayloadFormatDesc->wMaxMBperSecOneResTemporalScal = 0x0000;
  pVideoPayloadFormatDesc->wMaxMBperSecTwoResTemporalScal = 0x0000;
  pVideoPayloadFormatDesc->wMaxMBperSecThreeResTemporalScal = 0x0000;
  pVideoPayloadFormatDesc->wMaxMBperSecFourResTemporalScal = 0x0000;
  pVideoPayloadFormatDesc->wMaxMBperSecOneResTemporalQualityScal = 0x0000;
  pVideoPayloadFormatDesc->wMaxMBperSecTwoResTemporalQualityScal = 0x0000;
  pVideoPayloadFormatDesc->wMaxMBperSecThreeResTemporalQualityScal = 0x0000;
  pVideoPayloadFormatDesc->wMaxMBperSecFourResTemporalQualityScal = 0x0000;
  pVideoPayloadFormatDesc->wMaxMBperSecOneResTemporalSpatialScal = 0x0000;
  pVideoPayloadFormatDesc->wMaxMBperSecTwoResTemporalSpatialScal = 0x0000;
  pVideoPayloadFormatDesc->wMaxMBperSecThreeResTemporalSpatialScal = 0x0000;
  pVideoPayloadFormatDesc->wMaxMBperSecFourResTemporalSpatialScal = 0x0000;
  pVideoPayloadFormatDesc->wMaxMBperSecOneResFullScal = 0x0000;
  pVideoPayloadFormatDesc->wMaxMBperSecTwoResFullScal = 0x0000;
  pVideoPayloadFormatDesc->wMaxMBperSecThreeResFullScal = 0x0000;
  pVideoPayloadFormatDesc->wMaxMBperSecFourResFullScal = 0x0000;

#elif (USBD_UVC_USE_FRAME_BASE_H264 == 1U)
  pVideoPayloadFormatDesc->bDescriptorSubType = UX_DEVICE_CLASS_VIDEO_VS_FORMAT_FRAME_BASED;
  pVideoPayloadFormatDesc->bFormatIndex = 0x01U;
  pVideoPayloadFormatDesc->bNumFrameDescriptors = 0x01U;

  ux_utility_memory_copy(pVideoPayloadFormatDesc->pGuidFormat, usbd_uvc_guid, 16);

  pVideoPayloadFormatDesc->bBitsPerPixel = 16;
  pVideoPayloadFormatDesc->bDefaultFrameIndex = 0x01U;
  pVideoPayloadFormatDesc->bAspectRatioX = 0x00U;
  pVideoPayloadFormatDesc->bAspectRatioY = 0x00U;
  pVideoPayloadFormatDesc->bmInterlaceFlag = 0x00U;
  pVideoPayloadFormatDesc->bCopyProtect = 0x00U;
  pVideoPayloadFormatDesc->bVariableSize = 0x01U;
#else
  pVideoPayloadFormatDesc->bDescriptorSubType = UX_DEVICE_CLASS_VIDEO_VS_FORMAT_MJPEG;
  pVideoPayloadFormatDesc->bFormatIndex = 0x01U;
  pVideoPayloadFormatDesc->bNumFrameDescriptors = 0x01U;
  pVideoPayloadFormatDesc->bmFlags = 0x01U;
  pVideoPayloadFormatDesc->bDefaultFrameIndex = 0x01U;
  pVideoPayloadFormatDesc->bAspectRatioX = 0x00U;
  pVideoPayloadFormatDesc->bAspectRatioY = 0x00U;
  pVideoPayloadFormatDesc->bmInterfaceFlag = 0x00U;
  pVideoPayloadFormatDesc->bCopyProtect = 0x00U;
#endif /* USBD_UVC_USE_H264 */

  *config_framework_size += (uint32_t)sizeof(USBD_VIDEO_PAYLOAD_FORMAT_DESC);

  /* Append Class-specific VS (Video Streaming) Frame Descriptor to
     Configuration descriptor */
  pVideoFrameDesc = (USBD_VIDEO_FRAME_DESC *)((ULONG)p_config_framework + *config_framework_size);
  pVideoFrameDesc->bLength = (uint8_t)sizeof(USBD_VIDEO_FRAME_DESC);
  pVideoFrameDesc->bDescriptorType = UX_DEVICE_CLASS_VIDEO_CS_INTERFACE;

#if (USBD_UVC_USE_H264 == 1U)
  pVideoFrameDesc->bDescriptorSubType = UX_DEVICE_CLASS_VIDEO_VS_FRAME_H264;
  pVideoFrameDesc->bFrameIndex = 0x01U;
  pVideoFrameDesc->wWidth = UVC_FRAME_WIDTH;
  pVideoFrameDesc->wHeight = UVC_FRAME_HEIGHT;
  pVideoFrameDesc->wSARwidth = 0x0001U;
  pVideoFrameDesc->wSARheight = 0x0001U;
  pVideoFrameDesc->wProfile = 0x4240U;
  pVideoFrameDesc->bLevelIDC = 0x1EU;
  pVideoFrameDesc->wConstrainedToolset = 0x0000U;
  pVideoFrameDesc->bmSupportedUsages = 0x00000001U;
  pVideoFrameDesc->bmCapabilities = (1 << 0) |  /* Support for slice-based encoding*/
                                    (1 << 1);   /*Support for frame-based encoding*/
  pVideoFrameDesc->bmSVCCapabilities = 0x00000000U;
  pVideoFrameDesc->bmMVCCapabilities = 0x00000000U;
  pVideoFrameDesc->bNumFrameIntervals = 0x01U;

#if (USBD_HIGH_SPEED_SUPPORTED == 1U)
    pVideoFrameDesc->dwMinBitRate = UVC_MIN_BIT_RATE(UVC_CAM_FPS_HS);
    pVideoFrameDesc->dwMaxBitRate = UVC_MAX_BIT_RATE(UVC_CAM_FPS_HS);
    pVideoFrameDesc->dwDefaultFrameInterval = UVC_INTERVAL(UVC_CAM_FPS_HS);
    pVideoFrameDesc->dwFrameInterval = UVC_INTERVAL(UVC_CAM_FPS_HS);
#else
    pVideoFrameDesc->dwMinBitRate = UVC_MIN_BIT_RATE(UVC_CAM_FPS_FS);
    pVideoFrameDesc->dwMaxBitRate = UVC_MAX_BIT_RATE(UVC_CAM_FPS_FS);
    pVideoFrameDesc->dwDefaultFrameInterval = UVC_INTERVAL(UVC_CAM_FPS_FS);
    pVideoFrameDesc->dwFrameInterval = UVC_INTERVAL(UVC_CAM_FPS_FS);
#endif /*USBD_HIGH_SPEED_SUPPORTED*/

#elif (USBD_UVC_USE_FRAME_BASE_H264 == 1U)
  pVideoFrameDesc->bDescriptorSubType = UX_DEVICE_CLASS_VIDEO_VS_FRAME_FRAME_BASED;
  pVideoFrameDesc->bFrameIndex = 0x01U;
  pVideoFrameDesc->bmCapabilities = 0x00U;
  pVideoFrameDesc->wWidth = UVC_FRAME_WIDTH;
  pVideoFrameDesc->wHeight = UVC_FRAME_HEIGHT;

#if (USBD_HIGH_SPEED_SUPPORTED == 1U)
    pVideoFrameDesc->dwMinBitRate = UVC_MIN_BIT_RATE(UVC_CAM_FPS_HS);
    pVideoFrameDesc->dwMaxBitRate = UVC_MAX_BIT_RATE(UVC_CAM_FPS_HS);
    pVideoFrameDesc->dwDefaultFrameInterval = UVC_INTERVAL(UVC_CAM_FPS_HS);
    pVideoFrameDesc->dwFrameInterval = UVC_INTERVAL(UVC_CAM_FPS_HS);
#else
    pVideoFrameDesc->dwMinBitRate = UVC_MIN_BIT_RATE(UVC_CAM_FPS_FS);
    pVideoFrameDesc->dwMaxBitRate = UVC_MAX_BIT_RATE(UVC_CAM_FPS_FS);
    pVideoFrameDesc->dwDefaultFrameInterval = UVC_INTERVAL(UVC_CAM_FPS_FS);
    pVideoFrameDesc->dwFrameInterval = UVC_INTERVAL(UVC_CAM_FPS_FS);
#endif /* USBD_HIGH_SPEED_SUPPORTED */

  pVideoFrameDesc->dwBytesPerLine = 0x0U;
  pVideoFrameDesc->bFrameIntervalType = 0x01U;
#else
  pVideoFrameDesc->bDescriptorSubType = UX_DEVICE_CLASS_VIDEO_VS_FRAME_MJPEG;
  pVideoFrameDesc->bFrameIndex = 0x01U;
  pVideoFrameDesc->bmCapabilities = 0x02U;
  pVideoFrameDesc->wWidth = UVC_FRAME_WIDTH;
  pVideoFrameDesc->wHeight = UVC_FRAME_HEIGHT;

#if (USBD_HIGH_SPEED_SUPPORTED == 1U)
  pVideoFrameDesc->dwMinBitRate = UVC_MIN_BIT_RATE(UVC_CAM_FPS_HS);
  pVideoFrameDesc->dwMaxBitRate = UVC_MAX_BIT_RATE(UVC_CAM_FPS_HS);
  pVideoFrameDesc->dwDefaultFrameInterval = UVC_INTERVAL(UVC_CAM_FPS_HS);
  pVideoFrameDesc->dwFrameInterval = UVC_INTERVAL(UVC_CAM_FPS_HS);
#else
  pVideoFrameDesc->dwMinBitRate = UVC_MIN_BIT_RATE(UVC_CAM_FPS_FS);
  pVideoFrameDesc->dwMaxBitRate = UVC_MAX_BIT_RATE(UVC_CAM_FPS_FS);
  pVideoFrameDesc->dwDefaultFrameInterval = UVC_INTERVAL(UVC_CAM_FPS_FS);
  pVideoFrameDesc->dwFrameInterval = UVC_INTERVAL(UVC_CAM_FPS_FS);
#endif /* USBD_HIGH_SPEED_SUPPORTED */

  pVideoFrameDesc->dwMaxVideoFrameBufferSize = UVC_MAX_FRAME_SIZE;
  pVideoFrameDesc->bFrameIntervalType = 0x01U;
#endif /* USBD_UVC_USE_H264 */

  *config_framework_size += (uint32_t)sizeof(USBD_VIDEO_FRAME_DESC);

  /* USB Standard VS Interface  Descriptor - data transfer mode */
  /* Interface 1, Alternate Setting 1 */
  vs_interface_number = p_video_streaming_interface->interface_numbers;
  p_video_streaming_interface = (USB_DEVICE_INTERFACE_HANDLE *)p_video_streaming_interface->next_alt_setting;

  if (p_video_streaming_interface == UX_NULL)
  {
    return USBD_DESCRIPTOR_CLASS_BUILDER_ERROR;
  }

  /* Propagate the dynamically assigned interface number to the alternate setting */
  p_video_streaming_interface->interface_numbers = vs_interface_number;

  /* Append Video Streaming Interface descriptor to Configuration descriptor */
  usb_device_framework_set_interface((ULONG)p_config_framework, config_framework_size,
                                     p_video_itf_desc, p_video_streaming_interface);

  /* Get VIDEO Notification Endpoint */
  p_video_endpoint = (USB_DEVICE_ENDPOINT_HANDLE *)p_video_streaming_interface->device_endpoint;

  if (p_video_endpoint != UX_NULL)
  {
    /* Append Endpoint Notification descriptor to Configuration descriptor */
    usb_device_framework_set_endpoint((ULONG) p_config_framework, config_framework_size,
                                      p_video_ep_desc, p_video_endpoint, device_speed);
  }

  /* Update Config Descriptor */
  ((USBD_CONFIGURATION_DESC *)p_config_framework)->wTotalLength = *config_framework_size;
  ((USBD_CONFIGURATION_DESC *)p_config_framework)->bNumInterfaces += interface_index;

  return USBD_DESCRIPTOR_SUCCESS;
}
#endif /* USBD_VIDEO_CLASS_ACTIVATED */

#if (USBD_HID_MOUSE_ACTIVATED == 1U)
USB_DEVICE_ENDPOINT_HANDLE hid_mouse_endpoint[] =
{
  {
    USBD_HID_MOUSE_EPIN_ADDR,
    USBD_HID_MOUSE_EPIN_TYPE,
    USBD_HID_MOUSE_EPIN_FS_MPS,
    USBD_HID_MOUSE_EPIN_FS_BINTERVAL,
#if (USBD_HIGH_SPEED_SUPPORTED == 1U)
    USBD_HID_MOUSE_EPIN_HS_MPS,
    USBD_HID_MOUSE_EPIN_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  }
};

USB_DEVICE_INTERFACE_HANDLE hid_mouse_interface[] =
{
  {
    USBD_HID_MOUSE_ITF_NUMBERS,
    USBD_HID_MOUSE_ITF_ATL_SETTING,
    USBD_HID_MOUSE_ITF_EP_NUMBERS,
    USBD_HID_MOUSE_ITF_CLASS,
    USBD_HID_MOUSE_ITF_SUBCLASS,
    USBD_HID_MOUSE_ITF_PROTOCOL,
    USBD_HID_MOUSE_ITF_STR_DESC_IDX,
    0x00U,                /* Used for report descriptor length */
    hid_mouse_endpoint,   /* HID mouse endpoint */
    0x00U,                /* No alternate setting interface */
  }
};
#endif /* USBD_HID_MOUSE_ACTIVATED */

#if (USBD_HID_KEYBOARD_ACTIVATED == 1U)
USB_DEVICE_ENDPOINT_HANDLE hid_keyboard_endpoint[] =
{
  {
    USBD_HID_KEYBOARD_EPIN_ADDR,
    USBD_HID_KEYBOARD_EPIN_TYPE,
    USBD_HID_KEYBOARD_EPIN_FS_MPS,
    USBD_HID_KEYBOARD_EPIN_FS_BINTERVAL,
#if (USBD_HIGH_SPEED_SUPPORTED == 1U)
    USBD_HID_KEYBOARD_EPIN_HS_MPS,
    USBD_HID_KEYBOARD_EPIN_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  }
};

USB_DEVICE_INTERFACE_HANDLE hid_keyboard_interface[] =
{
  {
    USBD_HID_KEYBOARD_ITF_NUMBERS,
    USBD_HID_KEYBOARD_ITF_ATL_SETTING,
    USBD_HID_KEYBOARD_ITF_EP_NUMBERS,
    USBD_HID_KEYBOARD_ITF_CLASS,
    USBD_HID_KEYBOARD_ITF_SUBCLASS,
    USBD_HID_KEYBOARD_ITF_PROTOCOL,
    USBD_HID_KEYBOARD_ITF_STR_DESC_IDX,
    0x00U,
    hid_keyboard_endpoint,
    0x00U,
  }
};
#endif /* USBD_HID_KEYBOARD_ACTIVATED */

#if (USBD_HID_CUSTOM_ACTIVATED == 1U)
USB_DEVICE_ENDPOINT_HANDLE hid_custom_endpoint[] =
{
  {
    USBD_HID_CUSTOM_EPIN_ADDR,
    USBD_HID_CUSTOM_EPIN_TYPE,
    USBD_HID_CUSTOM_EPIN_FS_MPS,
    USBD_HID_CUSTOM_EPIN_FS_BINTERVAL,
#if (USBD_HIGH_SPEED_SUPPORTED == 1U)
    USBD_HID_CUSTOM_EPIN_HS_MPS,
    USBD_HID_CUSTOM_EPIN_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  },
  {
    USBD_HID_CUSTOM_EPOUT_ADDR,
    USBD_HID_CUSTOM_EPOUT_TYPE,
    USBD_HID_CUSTOM_EPOUT_FS_MPS,
    USBD_HID_CUSTOM_EPOUT_FS_BINTERVAL,
#if (USBD_HIGH_SPEED_SUPPORTED == 1U)
    USBD_HID_CUSTOM_EPOUT_HS_MPS,
    USBD_HID_CUSTOM_EPOUT_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  }
};

USB_DEVICE_INTERFACE_HANDLE hid_custom_interface[] =
{
  {
    USBD_HID_CUSTOM_ITF_NUMBERS,
    USBD_HID_CUSTOM_ITF_ATL_SETTING,
    USBD_HID_CUSTOM_ITF_EP_NUMBERS,
    USBD_HID_CUSTOM_ITF_CLASS,
    USBD_HID_CUSTOM_ITF_SUBCLASS,
    USBD_HID_CUSTOM_ITF_PROTOCOL,
    USBD_HID_CUSTOM_ITF_STR_DESC_IDX,
    0x00U,
    hid_custom_endpoint,
    0x00U,
  }
};
#endif /* USBD_HID_CUSTOM_ACTIVATED */

#if (USBD_CDC_ACM_CLASS_ACTIVATED == 1U)
/* Endpoint handles CONTROL for CDC_ACM */
USB_DEVICE_ENDPOINT_HANDLE cdc_acm_ctl_endpoints[] =
{
  {
    USBD_CDC_ACM_EPNTF_ADDR,
    USBD_CDC_ACM_EPNTF_TYPE,
    USBD_CDC_ACM_EPNTF_FS_MPS,
    USBD_CDC_ACM_EPNTF_FS_BINTERVAL,
#if (USBD_HIGH_SPEED_SUPPORTED == 1U)
    USBD_CDC_ACM_EPNTF_HS_MPS,
    USBD_CDC_ACM_EPNTF_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  }
};

/* Endpoint handles DATA for CDC_ACM */
USB_DEVICE_ENDPOINT_HANDLE cdc_acm_data_endpoints[] =
{
  {
    USBD_CDC_ACM_EPIN_ADDR,
    USBD_CDC_ACM_EPIN_TYPE,
    USBD_CDC_ACM_EPIN_FS_MPS,
    USBD_CDC_ACM_EPIN_FS_BINTERVAL,
#if (USBD_HIGH_SPEED_SUPPORTED == 1U)
    USBD_CDC_ACM_EPIN_HS_MPS,
    USBD_CDC_ACM_EPIN_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  },
  {
    USBD_CDC_ACM_EPOUT_ADDR,
    USBD_CDC_ACM_EPOUT_TYPE,
    USBD_CDC_ACM_EPOUT_FS_MPS,
    USBD_CDC_ACM_EPOUT_FS_BINTERVAL,
#if (USBD_HIGH_SPEED_SUPPORTED == 1U)
    USBD_CDC_ACM_EPOUT_HS_MPS,
    USBD_CDC_ACM_EPOUT_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  }
};

/* Interface handles for CDC_ACM */
USB_DEVICE_INTERFACE_HANDLE cdc_acm_interface[] =
{
  {
    USBD_CDC_ACM_CTL_NUMBERS,
    USBD_CDC_ACM_CTL_ATL_SETTING,
    USBD_CDC_ACM_CTL_EP_NUMBERS,
    USBD_CDC_ACM_CTL_CLASS,
    USBD_CDC_ACM_CTL_SUBCLASS,
    USBD_CDC_ACM_CTL_PROTOCOL,
    USBD_CDC_ACM_CTL_STR_DESC_IDX,
    0x00U,
    cdc_acm_ctl_endpoints,
    0x00U,
  },
  {
    USBD_CDC_ACM_DATA_NUMBERS,
    USBD_CDC_ACM_DATA_ATL_SETTING,
    USBD_CDC_ACM_DATA_EP_NUMBERS,
    USBD_CDC_ACM_DATA_CLASS,
    USBD_CDC_ACM_DATA_SUBCLASS,
    USBD_CDC_ACM_DATA_PROTOCOL,
    USBD_CDC_ACM_DATA_STR_DESC_IDX,
    0x00U,
    cdc_acm_data_endpoints,
    0x00U,
  }
};
#endif /* USBD_CDC_ACM_CLASS_ACTIVATED */

#if (USBD_MTP_CLASS_ACTIVATED == 1U)
/* Endpoint handles for MTP */
USB_DEVICE_ENDPOINT_HANDLE mtp_endpoints[] =
{
  {
    USBD_MTP_EPIN_ADDR,
    USBD_MTP_EPIN_TYPE,
    USBD_MTP_EPIN_FS_MPS,
    USBD_MTP_EPIN_FS_BINTERVAL,
#if (USBD_HIGH_SPEED_SUPPORTED == 1U)
    USBD_MTP_EPIN_HS_MPS,
    USBD_MTP_EPIN_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  },
  {
    USBD_MTP_EPOUT_ADDR,
    USBD_MTP_EPOUT_TYPE,
    USBD_MTP_EPOUT_FS_MPS,
    USBD_MTP_EPOUT_FS_BINTERVAL,
#if (USBD_HIGH_SPEED_SUPPORTED == 1U)
    USBD_MTP_EPOUT_HS_MPS,
    USBD_MTP_EPOUT_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  },
  {
    USBD_MTP_EPCTL_ADDR,
    USBD_MTP_EPCTL_TYPE,
    USBD_MTP_EPCTL_FS_MPS,
    USBD_MTP_EPCTL_FS_BINTERVAL,
#if (USBD_HIGH_SPEED_SUPPORTED == 1U)
    USBD_MTP_EPCTL_HS_MPS,
    USBD_MTP_EPCTL_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  }
};

/* Interface handles for MTP */
USB_DEVICE_INTERFACE_HANDLE mtp_interface[] =
{
  {
    USBD_MTP_ITF_NUMBERS,
    USBD_MTP_ITF_ATL_SETTING,
    USBD_MTP_ITF_EP_NUMBERS,
    USBD_MTP_ITF_CLASS,
    USBD_MTP_ITF_SUBCLASS,
    USBD_MTP_ITF_PROTOCOL,
    USBD_MTP_ITF_STR_DESC_IDX,
    0x00U,
    mtp_endpoints,
    0x00U,
  }
};
#endif /* USBD_MTP_CLASS_ACTIVATED */

#if (USBD_VIDEO_CLASS_ACTIVATED == 1U)

/* Endpoint handles for VIDEO */
USB_DEVICE_ENDPOINT_HANDLE video_endpoints[] =
{
  {
    USBD_VIDEO_EPIN_ADDR,
    USBD_VIDEO_EPIN_TYPE,
    USBD_VIDEO_EPIN_FS_MPS,
    USBD_VIDEO_EPIN_FS_BINTERVAL,
#if (USBD_HIGH_SPEED_SUPPORTED == 1U)
    USBD_VIDEO_EPIN_HS_MPS,
    USBD_VIDEO_EPIN_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  }
};

USB_DEVICE_INTERFACE_HANDLE next_alt_setting[] =
{
  {
    USBD_VIDEO_VS_AS_ITF_NUMBERS,
    USBD_VIDEO_VS_AS_ITF_ATL_SETTING,
    USBD_VIDEO_VS_AS_ITF_EP_NUMBERS,
    USBD_VIDEO_VS_AS_ITF_CLASS,
    USBD_VIDEO_VS_AS_ITF_SUBCLASS,
    USBD_VIDEO_VS_AS_ITF_PROTOCOL,
    USBD_VIDEO_VS_AS_ITF_STR_DESC_IDX,
    0x00U,
    video_endpoints,
    0x00U,
  }
};

/* Interface handles for VIDEO */
USB_DEVICE_INTERFACE_HANDLE video_interface[] =
{
  {
    USBD_VIDEO_ITF_NUMBERS,
    USBD_VIDEO_ITF_ATL_SETTING,
    USBD_VIDEO_ITF_EP_NUMBERS,
    USBD_VIDEO_ITF_CLASS,
    USBD_VIDEO_ITF_SUBCLASS,
    USBD_VIDEO_ITF_PROTOCOL,
    USBD_VIDEO_ITF_STR_DESC_IDX,
    0x00U,
    0x00U,
    0x00U,
    0x00U,
  },
  {
    USBD_VIDEO_VS_ITF_NUMBERS,
    USBD_VIDEO_VS_ITF_ATL_SETTING,
    USBD_VIDEO_VS_ITF_EP_NUMBERS,
    USBD_VIDEO_VS_ITF_CLASS,
    USBD_VIDEO_VS_ITF_SUBCLASS,
    USBD_VIDEO_VS_ITF_PROTOCOL,
    USBD_VIDEO_VS_ITF_STR_DESC_IDX,
    0x00U,
    0x00U,
    0x00U,
    next_alt_setting,
  }
};
#endif /* USBD_VIDEO_CLASS_ACTIVATED */
#if USBD_MSC_CLASS_ACTIVATED == 1U
USB_DEVICE_ENDPOINT_HANDLE msc_endpoints[] =
{
  {
    USBD_MSC_EPIN_ADDR,
    USBD_MSC_EPIN_TYPE,
    USBD_MSC_EPIN_FS_MPS,
    USBD_MSC_EPIN_FS_BINTERVAL,
#if USBD_HIGH_SPEED_SUPPORTED == 1U
    USBD_MSC_EPIN_HS_MPS,
    USBD_MSC_EPIN_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  },
  {
    USBD_MSC_EPOUT_ADDR,
    USBD_MSC_EPOUT_TYPE,
    USBD_MSC_EPOUT_FS_MPS,
    USBD_MSC_EPOUT_FS_BINTERVAL,
#if USBD_HIGH_SPEED_SUPPORTED == 1U
    USBD_MSC_EPOUT_HS_MPS,
    USBD_MSC_EPOUT_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  }
};

USB_DEVICE_INTERFACE_HANDLE msc_interface[] =
{
  {
    USBD_MSC_ITF_NUMBERS,
    USBD_MSC_ITF_ATL_SETTING,
    USBD_MSC_ITF_EP_NUMBERS,
    USBD_MSC_ITF_CLASS,
    USBD_MSC_ITF_SUBCLASS,
    USBD_MSC_ITF_PROTOCOL,
    USBD_MSC_ITF_STR_DESC_IDX,
    0x00U,
    msc_endpoints,
    0x00U,
  }
};
#endif /* USBD_MSC_CLASS_ACTIVATED */

#if USBD_DFU_CLASS_ACTIVATED == 1U
USB_DEVICE_INTERFACE_HANDLE dfu_interface[] =
{
  {
    USBD_DFU_ITF_NUMBERS,
    USBD_DFU_ITF_ATL_SETTING,
    USBD_DFU_ITF_EP_NUMBERS,
    USBD_DFU_ITF_CLASS,
    USBD_DFU_ITF_SUBCLASS,
    USBD_DFU_ITF_PROTOCOL,
    USBD_DFU_ITF_STR_DESC_IDX,
    0x00U,
    0x00U,
    0x00U,
  }
};
#endif /* USBD_DFU_CLASS_ACTIVATED */

#if USBD_AUDIO_CLASS_ACTIVATED == 1U
#ifdef USBD_AUDIO_EPINTERRUPT_SUPPORTED
USB_DEVICE_ENDPOINT_HANDLE audio_control_endpoints[] =
{
  {
    USBD_AUDIO_EPINTERRUPT_ADDR,
    USBD_AUDIO_EPINTERRUPT_TYPE,
    USBD_AUDIO_EPINTERRUPT_FS_MPS,
    USBD_AUDIO_EPINTERRUPT_FS_BINTERVAL,
#if USBD_HIGH_SPEED_SUPPORTED == 1U
    USBD_AUDIO_EPINTERRUPT_HS_MPS,
    USBD_AUDIO_EPINTERRUPT_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  }
};
#endif /* USBD_AUDIO_EPINTERRUPT_SUPPORTED */

USB_DEVICE_ENDPOINT_HANDLE audio_stream_in_endpoints[] =
{
  {
    USBD_AUDIO_EPIN_ADDR,
    USBD_AUDIO_DATA_EP_TYPE,
    USBD_AUDIO_EPIN_FS_MPS,
    USBD_AUDIO_EPIN_FS_BINTERVAL,
#if USBD_HIGH_SPEED_SUPPORTED == 1U
    USBD_AUDIO_EPIN_HS_MPS,
    USBD_AUDIO_EPIN_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  }
};

USB_DEVICE_ENDPOINT_HANDLE audio_stream_out_endpoints[] =
{
  {
    USBD_AUDIO_EPOUT_ADDR,
    USBD_AUDIO_DATA_EP_TYPE,
    USBD_AUDIO_EPOUT_FS_MPS,
    USBD_AUDIO_EPOUT_FS_BINTERVAL,
#if USBD_HIGH_SPEED_SUPPORTED == 1U
    USBD_AUDIO_EPOUT_HS_MPS,
    USBD_AUDIO_EPOUT_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  },
#ifdef USBD_AUDIO_EPFEEDBACK_SUPPORTED
  {
    USBD_AUDIO_EPFEEDBACK_ADDR,
    USBD_AUDIO_EPFEEDBACK_TYPE,
    USBD_AUDIO_EPFEEDBACK_FS_MPS,
    USBD_AUDIO_EPFEEDBACK_FS_BINTERVAL,
#if USBD_HIGH_SPEED_SUPPORTED == 1U
    USBD_AUDIO_EPFEEDBACK_HS_MPS,
    USBD_AUDIO_EPFEEDBACK_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  }
#endif /* USBD_AUDIO_EPFEEDBACK_SUPPORTED */
};

USB_DEVICE_INTERFACE_HANDLE audio_stream_in_alt_setting[] =
{
  {
    USBD_AUDIO_STREAM_IN_ITF_NUMBERS,
    USBD_AUDIO_STREAM_ALT_ITF_ATL_SETTING,
    USBD_AUDIO_STREAM_IN_ALT_EP_NUMBERS,
    USBD_AUDIO_STREAM_IN_ITF_CLASS,
    USBD_AUDIO_STREAM_IN_ITF_SUBCLASS,
    USBD_AUDIO_STREAM_IN_ITF_PROTOCOL,
    USBD_AUDIO_ITF_STR_DESC_IDX,
    0x00U,
    audio_stream_in_endpoints,
    0x00U,
  }
};

USB_DEVICE_INTERFACE_HANDLE audio_stream_out_alt_setting[] =
{
  {
    USBD_AUDIO_STREAM_OUT_ITF_NUMBERS,
    USBD_AUDIO_STREAM_ALT_ITF_ATL_SETTING,
    USBD_AUDIO_STREAM_OUT_ALT_EP_NUMBERS,
    USBD_AUDIO_STREAM_OUT_ITF_CLASS,
    USBD_AUDIO_STREAM_OUT_ITF_SUBCLASS,
    USBD_AUDIO_STREAM_OUT_ITF_PROTOCOL,
    USBD_AUDIO_ITF_STR_DESC_IDX,
    0x00U,
    audio_stream_out_endpoints,
    0x00U,
  }
};

USB_DEVICE_INTERFACE_HANDLE audio_interface[] =
{
  {
    USBD_AUDIO_CONTROL_ITF_NUMBERS,
    USBD_AUDIO_CONTROL_ITF_ATL_SETTING,
    USBD_AUDIO_CONTROL_ITF_EP_NUMBERS,
    USBD_AUDIO_CONTROL_ITF_CLASS,
    USBD_AUDIO_CONTROL_ITF_SUBCLASS,
    USBD_AUDIO_CONTROL_ITF_PROTOCOL,
    USBD_AUDIO_ITF_STR_DESC_IDX,
    0x00U,
#ifdef USBD_AUDIO_EPINTERRUPT_SUPPORTED
    audio_control_endpoints,
#else
    0x00U,
#endif
    0x00U,
  },
  {
    USBD_AUDIO_STREAM_IN_ITF_NUMBERS,
    USBD_AUDIO_STREAM_IN_ITF_ATL_SETTING,
    USBD_AUDIO_STREAM_IN_ITF_EP_NUMBERS,
    USBD_AUDIO_STREAM_IN_ITF_CLASS,
    USBD_AUDIO_STREAM_IN_ITF_SUBCLASS,
    USBD_AUDIO_STREAM_IN_ITF_PROTOCOL,
    USBD_AUDIO_ITF_STR_DESC_IDX,
    0x00U,
    0x00U,
    0x00U,
    audio_stream_in_alt_setting,
  },
  {
    USBD_AUDIO_STREAM_OUT_ITF_NUMBERS,
    USBD_AUDIO_STREAM_OUT_ITF_ATL_SETTING,
    USBD_AUDIO_STREAM_OUT_ITF_EP_NUMBERS,
    USBD_AUDIO_STREAM_OUT_ITF_CLASS,
    USBD_AUDIO_STREAM_OUT_ITF_SUBCLASS,
    USBD_AUDIO_STREAM_OUT_ITF_PROTOCOL,
    USBD_AUDIO_ITF_STR_DESC_IDX,
    0x00U,
    0x00U,
    0x00U,
    audio_stream_out_alt_setting,
  }
};
#endif /* USBD_AUDIO_CLASS_ACTIVATED */

#if USBD_PRINTER_CLASS_ACTIVATED == 1U
USB_DEVICE_ENDPOINT_HANDLE printer_endpoints[] =
{
  {
    USBD_PRINTER_EPIN_ADDR,
    USBD_PRINTER_EPIN_TYPE,
    USBD_PRINTER_EPIN_FS_MPS,
    USBD_PRINTER_EPIN_FS_BINTERVAL,
#if USBD_HIGH_SPEED_SUPPORTED == 1U
    USBD_PRINTER_EPIN_HS_MPS,
    USBD_PRINTER_EPIN_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  },
  {
    USBD_PRINTER_EPOUT_ADDR,
    USBD_PRINTER_EPOUT_TYPE,
    USBD_PRINTER_EPOUT_FS_MPS,
    USBD_PRINTER_EPOUT_FS_BINTERVAL,
#if USBD_HIGH_SPEED_SUPPORTED == 1U
    USBD_PRINTER_EPOUT_HS_MPS,
    USBD_PRINTER_EPOUT_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  }
};

USB_DEVICE_INTERFACE_HANDLE printer_interface[] =
{
  {
    USBD_PRINTER_ITF_NUMBERS,
    USBD_PRINTER_ITF_ATL_SETTING,
    USBD_PRINTER_ITF_EP_NUMBERS,
    USBD_PRINTER_ITF_CLASS,
    USBD_PRINTER_ITF_SUBCLASS,
    USBD_PRINTER_ITF_PROTOCOL,
    USBD_PRINTER_ITF_STR_DESC_IDX,
    0x00U,
    printer_endpoints,
    0x00U,
  }
};
#endif /* USBD_PRINTER_CLASS_ACTIVATED */

#if USBD_CCID_CLASS_ACTIVATED == 1U
USB_DEVICE_ENDPOINT_HANDLE ccid_endpoints[] =
{
  {
    USBD_CCID_EPIN_ADDR,
    USBD_CCID_EPIN_TYPE,
    USBD_CCID_EPIN_FS_MPS,
    USBD_CCID_EPIN_FS_BINTERVAL,
#if USBD_HIGH_SPEED_SUPPORTED == 1U
    USBD_CCID_EPIN_HS_MPS,
    USBD_CCID_EPIN_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  },
  {
    USBD_CCID_EPOUT_ADDR,
    USBD_CCID_EPOUT_TYPE,
    USBD_CCID_EPOUT_FS_MPS,
    USBD_CCID_EPOUT_FS_BINTERVAL,
#if USBD_HIGH_SPEED_SUPPORTED == 1U
    USBD_CCID_EPOUT_HS_MPS,
    USBD_CCID_EPOUT_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  },
  {
    USBD_CCID_EPCTL_ADDR,
    USBD_CCID_EPCTL_TYPE,
    USBD_CCID_EPCTL_FS_MPS,
    USBD_CCID_EPCTL_FS_BINTERVAL,
#if USBD_HIGH_SPEED_SUPPORTED == 1U
    USBD_CCID_EPCTL_HS_MPS,
    USBD_CCID_EPCTL_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  }
};

USB_DEVICE_INTERFACE_HANDLE ccid_interface[] =
{
  {
    USBD_CCID_ITF_NUMBERS,
    USBD_CCID_ITF_ATL_SETTING,
    USBD_CCID_ITF_EP_NUMBERS,
    USBD_CCID_ITF_CLASS,
    USBD_CCID_ITF_SUBCLASS,
    USBD_CCID_ITF_PROTOCOL,
    USBD_CCID_ITF_STR_DESC_IDX,
    0x00U,
    ccid_endpoints,
    0x00U,
  }
};
#endif /* USBD_CCID_CLASS_ACTIVATED */
