/**
 * Return if SW component has been enabled in the project (or at least its dependencies)
 * @param {object} used_components used components returned by SWProjectAPI.getUsedComponents getter
 * @param {string} component component to check
 */
function helper_usbx_class_is_selected(used_components, component) {
  let result = 0;
  try {
    used_components.forEach((comp) => {
      if (
        comp["cgroup"] === "STM32CubeMX2 Codegen" &&
        comp["csub"].toUpperCase() === component.toUpperCase()
      ) {
        result = 1;
        return;
      }
    });
  } catch (e) {
    console.error(`[ERROR] usbx_helpers: ${e}`);
  }
  return result;
}

/**
 * Return if SW component has been enabled in the project (or at least its dependencies)
 * @param {object} used_components used components returned by SWProjectAPI.getUsedComponents getter
 * @param {string} component component to check
 */
function helper_usbx_core_is_enabled(used_components, component) {
  let result = 0;
  try {
    used_components.forEach((comp) => {
      if (
        comp["cgroup"] === "USBX Core" &&
        comp["csub"].toUpperCase() === component.toUpperCase()
      ) {
        result = 1;
        return;
      }
    });
  } catch (e) {
    console.error(`[ERROR] usbx_helpers: ${e}`);
  }
  return result;
}

/**
 * Return if SW component has been enabled in the project (or at least its dependencies)
 * @param {object} used_components used components returned by SWProjectAPI.getUsedComponents getter
 * @param {string} component component to check
 */
function helper_usbx_controllers_is_enabled(used_components, component) {
  let result = 0;
  try {
    used_components.forEach((comp) => {
      if (
        comp["cgroup"] === "USBX Controllers Driver" &&
        comp["csub"].toUpperCase() === component.toUpperCase()
      ) {
        result = 1;
        return;
      }
    });
  } catch (e) {
    console.error(`[ERROR] usbx_helpers: ${e}`);
  }
  return result;
}

/**
 * Return the length of an array (used for Handlebars)
 * @param {Array} array The array to count
 * @returns {number} The length of the array, or 0 if not an array
 */
function helper_usbx_array_length(array) {
  if (Array.isArray(array)) {
    return array.length;
  }
  return 0;
}
/**
 * Greater than (a > b)
 */
function helper_usbx_is_greater(a, b) {
  return a > b;
}

/**
 * Checks whether a device-context binding refers to an IP running in HOST mode.
 * In DRD configurations, a "device instance" selector can legitimately point to a controller configured as HOST.
 * @param {object|string} inputParameter - The bound device instance or raw selection string.
 * @returns {boolean}
 */
function helper_usbx_is_device_instance(inputParameter) {
  const selection = typeof inputParameter === 'string' ? inputParameter : inputParameter?.selection;
  return (typeof selection === 'string' && selection.includes('HOST'));
}

/**
 * Checks whether a host-context binding refers to an IP running in DEVICE mode.
 * In DRD configurations, a "host instance" selector can legitimately point to a controller configured as DEVICE.
 * @param {object|string} item - The host_instance item or raw selection string.
 * @returns {boolean}
 */
function helper_usbx_is_host_instance(item) {
  const selection = typeof item === 'string' ? item : item?.select_the_host_instance?.selection;
  return (typeof selection === 'string' && selection.includes('DEVICE'));
}

/**
 * Return ip_name in a string format.
 * @param {string} value
 * @returns {string}
 */
function helper_usbx_get_ip_name(value) {
  if (typeof value !== 'string') {
    return '';
  }
  return value.replace(/\./g, '_');
}

/**
 * Checks whether the selected USBX OS mode is FreeRTOS.
 * @param {string} value
 * @returns {boolean}
 */
function helper_usbx_is_freertos(value) {
  return value === 'FreeRTOS';
}

/**
 * Checks whether at least one USBX device class is enabled in the configuration.
 * This mirrors the enable condition used by the JSON schema for device init code generation.
 * @param {object} deviceClassConfig
 * @returns {boolean}
 */
function helper_usbx_has_device_class_selected(deviceClassConfig) {
  return Boolean(
    deviceClassConfig?.hid_class === true ||
      deviceClassConfig?.cdc_acm_class === true ||
      deviceClassConfig?.msc_class === true ||
      deviceClassConfig?.pima_mtp_class === true ||
      deviceClassConfig?.dfu_class === true ||
      deviceClassConfig?.ccid_class === true ||
      deviceClassConfig?.printer_class === true ||
      deviceClassConfig?.video_class === true ||
      deviceClassConfig?.audio_class === true,
  );
}

/**
 * Checks whether at least one USBX host class is enabled in the configuration.
 * This mirrors the enable condition used by the JSON schema for host init code generation.
 * @param {object} hostClassConfig
 * @returns {boolean}
 */
function helper_usbx_has_host_class_selected(hostClassConfig) {
  return Boolean(
    hostClassConfig?.hid_class === true ||
      hostClassConfig?.cdc_acm_class === true ||
      hostClassConfig?.msc_class === true ||
      hostClassConfig?.hub_class === true ||
      hostClassConfig?.audio_class === true ||
      hostClassConfig?.video_class === true ||
      hostClassConfig?.printer_class === true ||
      hostClassConfig?.gser_class === true ||
      hostClassConfig?.pima_mtp_class === true ||
      hostClassConfig?.prolific_class === true ||
      hostClassConfig?.swar_class === true,
  );
}

function helper_usbx_get_nested_value(value, pathExpression) {
  if (!value || typeof value !== 'object' || typeof pathExpression !== 'string') {
    return undefined;
  }

  return pathExpression.split('.').reduce((currentValue, pathSegment) => currentValue?.[pathSegment], value);
}

const USBX_DEVICE_ENDPOINT_ALLOCATIONS = [
  {
    path: 'mouse_interface.usbd_hid_mouse_endpoint_in_addr',
    direction: 'in',
    isEnabled: (deviceClassConfig) => deviceClassConfig?.device_mouse === true,
  },
  {
    path: 'keyboard_interface.usbd_hid_keyboard_endpoint_in_addr',
    direction: 'in',
    isEnabled: (deviceClassConfig) => deviceClassConfig?.device_keyboard === true,
  },
  {
    path: 'custom_interface.usbd_hid_custom_endpoint_in_addr',
    direction: 'in',
    isEnabled: (deviceClassConfig) => deviceClassConfig?.device_custom === true,
  },
  {
    path: 'custom_interface.more.usbd_hid_custom_endpoint_out_addr',
    direction: 'out',
    isEnabled: (deviceClassConfig) =>
      deviceClassConfig?.device_custom === true &&
      deviceClassConfig?.custom_interface?.more?.ux_device_class_hid_interrupt_out_support === true,
  },
  {
    path: 'cdc_acm_interface.usbd_cdc_acm_endpoint_in_cmd_addr',
    direction: 'in',
    isEnabled: (deviceClassConfig) => deviceClassConfig?.cdc_acm_class === true,
  },
  {
    path: 'cdc_acm_interface.usbd_cdc_acm_endpoint_in_addr',
    direction: 'in',
    isEnabled: (deviceClassConfig) => deviceClassConfig?.cdc_acm_class === true,
  },
  {
    path: 'cdc_acm_interface.usbd_cdc_acm_endpoint_out_addr',
    direction: 'out',
    isEnabled: (deviceClassConfig) => deviceClassConfig?.cdc_acm_class === true,
  },
  {
    path: 'msc_interface.usbd_msc_endpoint_in_addr',
    direction: 'in',
    isEnabled: (deviceClassConfig) => deviceClassConfig?.msc_class === true,
  },
  {
    path: 'msc_interface.usbd_msc_endpoint_out_addr',
    direction: 'out',
    isEnabled: (deviceClassConfig) => deviceClassConfig?.msc_class === true,
  },
  {
    path: 'audio_interface.usbd_audio_endpoint_in_addr',
    direction: 'in',
    isEnabled: (deviceClassConfig) => deviceClassConfig?.audio_class === true,
  },
  {
    path: 'audio_interface.usbd_audio_endpoint_out_addr',
    direction: 'out',
    isEnabled: (deviceClassConfig) => deviceClassConfig?.audio_class === true,
  },
  {
    path: 'audio_interface.usbd_audio_endpoint_interrupt_addr',
    direction: 'in',
    isEnabled: (deviceClassConfig) =>
      deviceClassConfig?.audio_class === true &&
      deviceClassConfig?.audio_core?.audio_control_interrupt_enable === true,
  },
  {
    path: 'audio_interface.usbd_audio_endpoint_feedback_addr',
    direction: 'in',
    isEnabled: (deviceClassConfig) =>
      deviceClassConfig?.audio_class === true &&
      deviceClassConfig?.audio_core?.audio_feedback_support === true,
  },
  {
    path: 'video_interface.usbd_video_endpoint_in_addr',
    direction: 'in',
    isEnabled: (deviceClassConfig) => deviceClassConfig?.video_class === true,
  },
  {
    path: 'printer_interface.usbd_printer_endpoint_in_addr',
    direction: 'in',
    isEnabled: (deviceClassConfig) => deviceClassConfig?.printer_class === true,
  },
  {
    path: 'printer_interface.usbd_printer_endpoint_out_addr',
    direction: 'out',
    isEnabled: (deviceClassConfig) => deviceClassConfig?.printer_class === true,
  },
  {
    path: 'ccid_interface.usbd_ccid_endpoint_in_cmd_addr',
    direction: 'in',
    isEnabled: (deviceClassConfig) => deviceClassConfig?.ccid_class === true,
  },
  {
    path: 'ccid_interface.usbd_ccid_endpoint_in_addr',
    direction: 'in',
    isEnabled: (deviceClassConfig) => deviceClassConfig?.ccid_class === true,
  },
  {
    path: 'ccid_interface.usbd_ccid_endpoint_out_addr',
    direction: 'out',
    isEnabled: (deviceClassConfig) => deviceClassConfig?.ccid_class === true,
  },
  {
    path: 'pima_mtp_interface.usbd_pima_endpoint_in_addr',
    direction: 'in',
    isEnabled: (deviceClassConfig) => deviceClassConfig?.pima_mtp_class === true,
  },
  {
    path: 'pima_mtp_interface.usbd_pima_endpoint_out_addr',
    direction: 'out',
    isEnabled: (deviceClassConfig) => deviceClassConfig?.pima_mtp_class === true,
  },
  {
    path: 'pima_mtp_interface.usbd_pima_endpoint_cmd_addr',
    direction: 'in',
    isEnabled: (deviceClassConfig) => deviceClassConfig?.pima_mtp_class === true,
  },
];

function helper_usbx_default_device_endpoint_address(deviceClassConfig, endpointPath) {
  try {
    const targetEntry = USBX_DEVICE_ENDPOINT_ALLOCATIONS.find((entry) => entry.path === endpointPath);

    if (!targetEntry) {
      return 1;
    }

    const targetClassPath = endpointPath.split('.')[0];
    const enabledEntries = USBX_DEVICE_ENDPOINT_ALLOCATIONS.filter(
      (entry) => entry.direction === targetEntry.direction && entry.isEnabled(deviceClassConfig) === true,
    );

    const reservedAddresses = new Set();
    for (const entry of enabledEntries) {
      if (entry.path.split('.')[0] === targetClassPath) {
        continue;
      }

      const entryAddress = Number(helper_usbx_get_nested_value(deviceClassConfig, entry.path));
      if (Number.isInteger(entryAddress) && entryAddress > 0) {
        reservedAddresses.add(entryAddress);
      }
    }

    const classEntries = enabledEntries.filter((entry) => entry.path.split('.')[0] === targetClassPath);
    const resolvedAddresses = new Map();

    for (const entry of classEntries) {
      const entryAddress = Number(helper_usbx_get_nested_value(deviceClassConfig, entry.path));
      let resolvedAddress = entryAddress;

      if (!Number.isInteger(entryAddress) || entryAddress <= 0 || reservedAddresses.has(entryAddress)) {
        resolvedAddress = 1;
        while (reservedAddresses.has(resolvedAddress)) {
          resolvedAddress += 1;
        }
      }

      reservedAddresses.add(resolvedAddress);
      resolvedAddresses.set(entry.path, resolvedAddress);

      if (entry.path === endpointPath) {
        return resolvedAddress;
      }
    }

    return resolvedAddresses.get(endpointPath) ?? 1;
  } catch (e) {
    console.error(`[ERROR] usbx_helpers: ${e}`);
    return 1;
  }
}

function helper_usbx_has_duplicate_device_endpoint_address(deviceClassConfig, endpointPath) {
  try {
    const targetEntry = USBX_DEVICE_ENDPOINT_ALLOCATIONS.find((entry) => entry.path === endpointPath);

    if (!targetEntry || targetEntry.isEnabled(deviceClassConfig) !== true) {
      return false;
    }

    const targetAddress = Number(helper_usbx_get_nested_value(deviceClassConfig, endpointPath));
    if (!Number.isInteger(targetAddress) || targetAddress <= 0) {
      return false;
    }

    let duplicatesCount = 0;

    for (const entry of USBX_DEVICE_ENDPOINT_ALLOCATIONS) {
      if (entry.direction !== targetEntry.direction || entry.isEnabled(deviceClassConfig) !== true) {
        continue;
      }

      const entryAddress = Number(helper_usbx_get_nested_value(deviceClassConfig, entry.path));
      if (entryAddress === targetAddress) {
        duplicatesCount += 1;
      }

      if (duplicatesCount > 1) {
        return true;
      }
    }

    return false;
  } catch (e) {
    console.error(`[ERROR] usbx_helpers: ${e}`);
    return false;
  }
}

/**
 * Checks whether any instance selection references a high-speed USB instance.
 * @param {unknown} value
 * @returns {boolean}
 */
const USBX_MAX_INSTANCE_RECURSION_DEPTH = 10;
const USBX_HIGH_SPEED_INSTANCE_PATTERN = /(?:USB[\w.]*_HS|OTG_HS)(?:$|[^A-Za-z0-9])/i;
const USBX_FULL_SPEED_INSTANCE_PATTERN = /(?:USB[\w.]*_FS|OTG_FS)(?:$|[^A-Za-z0-9])/i;

function helper_usbx_has_high_speed_instance_selected(value, depth = 0) {
  if (depth > USBX_MAX_INSTANCE_RECURSION_DEPTH) {
    return false;
  }

  if (typeof value === 'string') {
    return USBX_HIGH_SPEED_INSTANCE_PATTERN.test(value);
  }

  if (Array.isArray(value)) {
    return value.some((entry) => helper_usbx_has_high_speed_instance_selected(entry, depth + 1));
  }

  if (value && typeof value === 'object') {
    if (typeof value.selection === 'string') {
      return USBX_HIGH_SPEED_INSTANCE_PATTERN.test(value.selection);
    }

    if (value.select_the_device_instance) {
      return helper_usbx_has_high_speed_instance_selected(value.select_the_device_instance, depth + 1);
    }

    if (value.select_the_host_instance) {
      return helper_usbx_has_high_speed_instance_selected(value.select_the_host_instance, depth + 1);
    }
  }

  return false;
}

/**
 * Checks whether any instance selection references a full-speed USB instance.
 * @param {unknown} value
 * @returns {boolean}
 */
function helper_usbx_has_full_speed_instance_selected(value, depth = 0) {
  if (depth > USBX_MAX_INSTANCE_RECURSION_DEPTH) {
    return false;
  }

  if (typeof value === 'string') {
    return USBX_FULL_SPEED_INSTANCE_PATTERN.test(value);
  }

  if (Array.isArray(value)) {
    return value.some((entry) => helper_usbx_has_full_speed_instance_selected(entry, depth + 1));
  }

  if (value && typeof value === 'object') {
    if (typeof value.selection === 'string') {
      return USBX_FULL_SPEED_INSTANCE_PATTERN.test(value.selection);
    }

    if (value.select_the_device_instance) {
      return helper_usbx_has_full_speed_instance_selected(value.select_the_device_instance, depth + 1);
    }

    if (value.select_the_host_instance) {
      return helper_usbx_has_full_speed_instance_selected(value.select_the_host_instance, depth + 1);
    }
  }

  return false;
}

/**
 * Returns the USB IN endpoint address for the provided endpoint number.
 * Non-numeric values are ignored. If no valid numeric value is provided, returns "0x80U".
 * Valid endpoint numbers are in the range 0 to 15.
 * @param {...number} values
 * @returns {string}
 */
function helper_usb_ep_in_addr(...values) {
  const numericValues = values
    .map((value) => Number(value))
    .filter((value) => Number.isInteger(value) && value >= 0 && value <= 15);

  if (numericValues.length === 0) {
    return "0x80U";
  }

  return `0x${(0x80 | numericValues[0]).toString(16).toUpperCase()}U`;
}

/**
 * Returns the USB OUT endpoint address for the provided endpoint number.
 * Non-numeric values are ignored. If no valid numeric value is provided, returns "0x00U".
 * Valid endpoint numbers are in the range 0 to 15.
 * @param {...number} values
 * @returns {string}
 */
function helper_usb_ep_out_addr(...values) {
  const numericValues = values
    .map((value) => Number(value))
    .filter((value) => Number.isInteger(value) && value >= 0 && value <= 15);

  if (numericValues.length === 0) {
    return "0x00U";
  }

  return `0x${numericValues[0].toString(16).toUpperCase().padStart(2, "0")}U`;
}

module.exports = {
  helper_usbx_class_is_selected,
  helper_usbx_core_is_enabled,
  helper_usbx_controllers_is_enabled,
  helper_usbx_array_length,
  helper_usbx_is_greater,
  helper_usbx_is_host_instance,
  helper_usbx_is_device_instance,
  helper_usbx_get_ip_name,
  helper_usbx_is_freertos,
  helper_usbx_has_device_class_selected,
  helper_usbx_has_host_class_selected,
  helper_usbx_default_device_endpoint_address,
  helper_usbx_has_duplicate_device_endpoint_address,
  helper_usbx_has_high_speed_instance_selected,
  helper_usbx_has_full_speed_instance_selected,
  helper_usb_ep_in_addr,
  helper_usb_ep_out_addr,
};