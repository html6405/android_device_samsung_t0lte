/*
   Copyright (c) 2016, The Linux Foundation. All rights reserved.
   Copyright (c) 2017-2020, The LineageOS Project. All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.
   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/strings.h>

#include "property_service.h"
#include "vendor_init.h"

using android::base::GetProperty;
using android::base::ReadFileToString;
using android::base::Trim;
using android::init::property_set;

#define SERIAL_NUMBER_FILE "/efs/FactoryApp/serial_no"

void property_override(char const prop[], char const value[])
{
    prop_info *pi;

    pi = (prop_info*) __system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

void property_override_dual(char const system_prop[], char const vendor_prop[], char const value[])
{
    property_override(system_prop, value);
    property_override(vendor_prop, value);
}

void vendor_load_properties()
{
    const std::string bootloader = GetProperty("ro.bootloader", "");
    const std::string platform = GetProperty("ro.board.platform", "");

    char const *serial_number_file = SERIAL_NUMBER_FILE;
    std::string serial_number;

    if (ReadFileToString(serial_number_file, &serial_number)) {
        serial_number = Trim(serial_number);
        property_override("ro.serialno", serial_number.c_str());
    }

    if (bootloader.find("I317") != std::string::npos) {
        /* SGH-i317 (AT&T) */
        property_override_dual("ro.product.model", "ro.vendor.product.model", "SGH-I317");
        property_override_dual("ro.product.device", "ro.vendor.product.device", "t0lteatt");
        property_override_dual("ro.product.device", "ro.vendor.product.name", "t0lteatt");
        property_override_dual("ro.build.description", "ro.vendor.build.description", "t0lteuc-user 4.4.2 KOT49H I317UCUCNJ1 release-keys");
        property_override_dual("ro.build.fingerprint", "ro.vendor.build.fingerprint", "samsung/t0lteuc/t0lteatt:4.4.2/KOT49H/I317UCUCNJ1:user/release-keys");
        property_override("ro.build.product", "t0lteatt");
    } else if (bootloader.find("I605") != std::string::npos) {
        /* SCH-I605 (Verizon) */
        property_override_dual("ro.product.model", "ro.vendor.product.model", "SCH-I605");
        property_override_dual("ro.product.device", "ro.vendor.product.device", "t0ltevzw");
        property_override_dual("ro.product.name", "ro.vendor.product.name", "t0ltevzw");
        property_override_dual("ro.build.description", "ro.vendor.build.description", "t0ltevzw-user 4.4.2 KOT49H I605VRUFND7 release-keys");
        property_override_dual("ro.build.fingerprint", "ro.vendor.build.fingerprint", "Verizon/t0ltevzw/t0ltevzw:4.4.2/KOT49H/I605VRUFND7:user/release-keys");
        property_override("ro.build.product", "t0ltevzw");
    } else if (bootloader.find("T889") != std::string::npos) {
        /* SGH-T889 (T-Mobile) */
        property_override_dual("ro.product.model", "ro.vendor.product.model", "SGH-T889");
        property_override_dual("ro.product.device", "ro.vendor.product.device", "t0ltetmo");
        property_override_dual("ro.product.name", "ro.vendor.product.name", "t0ltetmo");
        property_override_dual("ro.build.description", "ro.vendor.build.description", "t0ltetmo-user 4.3 JSS15J T889UVUCMK7 release-keys");
        property_override_dual("ro.build.fingerprint", "ro.vendor.build.fingerprint", "samsung/t0ltetmo/t0ltetmo:4.3/JSS15J/T889UVUCMK7:user/release-keys");
        property_override("ro.build.product", "t0ltetmo");
    } else if (bootloader.find("L900") != std::string::npos) {
        /* SCH-L900 (Sprint) */
        property_override_dual("ro.product.model", "ro.vendor.product.model", "SPH-L900");
        property_override_dual("ro.product.device", "ro.vendor.product.device", "t0ltespr");
        property_override_dual("ro.product.name", "ro.vendor.product.name", "t0ltespr");
        property_override_dual("ro.build.description", "ro.vendor.build.description", "t0ltespr-user 4.4.2 KOT49H L900VPUCNE2 release-keys");
        property_override_dual("ro.build.fingerprint", "ro.vendor.build.fingerprint", "samsung/t0ltespr/t0ltespr:4.4.2/KOT49H/L900VPUCNE2:user/release-keys");
        property_override("ro.build.product", "t0ltespr");
    } else if (bootloader.find("E250S") != std::string::npos) {
        /* SHV-E250S (SK Telecom) */
        property_override_dual("ro.product.model", "ro.vendor.product.model", "SHV-E250S");
        property_override_dual("ro.product.device", "ro.vendor.product.device", "t0lteskt");
        property_override_dual("ro.product.name", "ro.vendor.product.name", "t0lteskt");
        property_override_dual("ro.build.description", "ro.vendor.build.description", "t0lteskt-user 4.4.2 KOT49H E250SKSUKNI2 release-keys");
        property_override_dual("ro.build.fingerprint", "ro.vendor.build.fingerprint", "samsung/t0lteskt/t0lteskt:4.4.2/KOT49H/E250SKSUKNI2:user/release-keys");
        property_override("ro.build.product", "t0lteskt");
    } else if (bootloader.find("E250K") != std::string::npos) {
        /* SHV-E250K (KT Olleh) */
        property_override_dual("ro.product.model", "ro.vendor.product.model", "SHV-E250K");
        property_override_dual("ro.product.device", "ro.vendor.product.device", "t0ltektt");
        property_override_dual("ro.product.name", "ro.vendor.product.name", "t0ltektt");
        property_override_dual("ro.build.description", "ro.vendor.build.description", "t0ltektt-user 4.4.2 KOT49H E250KKTUKNI1 release-keys");
        property_override_dual("ro.build.fingerprint", "ro.vendor.build.fingerprint", "samsung/t0ltektt/t0ltektt:4.4.2/KOT49H/E250KKTUKNI1:user/release-keys");
        property_override("ro.build.product", "t0ltektt");
    } else if (bootloader.find("E250L") != std::string::npos) {
        /* SHV-E250L (LG Uplus) */
        property_override_dual("ro.product.model", "ro.vendor.product.model", "SHV-E250L");
        property_override_dual("ro.product.device", "ro.vendor.product.device", "t0ltelgt");
        property_override_dual("ro.product.name", "ro.vendor.product.name", "t0ltelgt");
        property_override_dual("ro.build.description", "ro.vendor.build.description", "t0ltelgt-user 4.4.2 KOT49H E250LKLUKNL1 release-keys");
        property_override_dual("ro.build.fingerprint", "ro.vendor.build.fingerprint", "samsung/t0ltelgt/t0ltelgt:4.4.2/KOT49H/E250LKLUKNL1:user/release-keys");
        property_override("ro.build.product", "t0ltelgt");
    } else if (bootloader.find("SC02E") != std::string::npos) {
        /* SC-02E (SGH-N025) (NTT DoCoMo) */
	property_override_dual("ro.product.model", "ro.vendor.product.model", "SC-02E");
	property_override_dual("ro.product.device", "ro.vendor.product.device", "t0ltedcm");
	property_override_dual("ro.product.name", "ro.vendor.product.name", "t0ltedcm");
	property_override_dual("ro.build.description", "ro.vendor.build.description", "t0ltedcm-user 4.3 JSS15J SC02EOMUBNJ2 release-keys");
	property_override_dual("ro.build.fingerprint", "ro.vendor.build.fingerprint", "samsung/t0ltedcm/t0ltedcm:4.3/JSS15J/SC02EOMUBNJ2:user/release-keys");
	property_override("ro.build.product", "t0ltedcm");
    } else {
        /* Fall back to GT-N7105 (International LTE) */
        property_override_dual("ro.product.model", "ro.vendor.product.model", "GT-N7105");
        property_override_dual("ro.product.device", "ro.vendor.product.device", "t0lte");
        property_override_dual("ro.product.name", "ro.vendor.product.name", "t0lte");
        property_override_dual("ro.build.description", "ro.vendor.build.description", "t0ltexx-user 4.4.2 KOT49H N7105XXUFOB2 release-keys");
        property_override_dual("ro.build.fingerprint", "ro.vendor.build.fingerprint", "samsung/t0ltexx/t0lte:4.4.2/KOT49H/N7105XXUFOB2:user/release-keys");
        property_override("ro.build.product", "t0lte");
    }

    const std::string device = GetProperty("ro.product.device", "");
    LOG(INFO) << "Found bootloader " << bootloader << ". " << "Setting build properties for " << device << ".\n";
}
