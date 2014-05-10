/*
 * domain_addr.h: helper APIs for managing domain device addresses
 *
 * Copyright (C) 2006-2014 Red Hat, Inc.
 * Copyright (C) 2006 Daniel P. Berrange
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#ifndef __DOMAIN_ADDR_H__
# define __DOMAIN_ADDR_H__

# include "domain_conf.h"

# define VIR_PCI_ADDRESS_SLOT_LAST 31
# define VIR_PCI_ADDRESS_FUNCTION_LAST 7

typedef enum {
   VIR_PCI_CONNECT_HOTPLUGGABLE = 1 << 0,
   /* This bus supports hot-plug */
   VIR_PCI_CONNECT_SINGLESLOT   = 1 << 1,
   /* This "bus" has only a single downstream slot/port */

   VIR_PCI_CONNECT_TYPE_PCI     = 1 << 2,
   /* PCI devices can connect to this bus */
   VIR_PCI_CONNECT_TYPE_PCIE    = 1 << 3,
   /* PCI Express devices can connect to this bus */
   VIR_PCI_CONNECT_TYPE_EITHER_IF_CONFIG = 1 << 4,
   /* PCI *and* PCIe devices allowed, if the address
    * was specified in the config by the user
    */
} virDomainPCIConnectFlags;

typedef struct {
    virDomainControllerModelPCI model;
    /* flags an min/max can be computed from model, but
     * having them ready makes life easier.
     */
    virDomainPCIConnectFlags flags;
    size_t minSlot, maxSlot; /* usually 0,0 or 1,31 */
    /* Each bit in a slot represents one function on that slot. If the
     * bit is set, that function is in use by a device.
     */
    uint8_t slots[VIR_PCI_ADDRESS_SLOT_LAST + 1];
} virDomainPCIAddressBus;
typedef virDomainPCIAddressBus *virDomainPCIAddressBusPtr;

struct _virDomainPCIAddressSet {
    virDomainPCIAddressBus *buses;
    size_t nbuses;
    virDevicePCIAddress lastaddr;
    virDomainPCIConnectFlags lastFlags;
    bool dryRun;          /* on a dry run, new buses are auto-added
                             and addresses aren't saved in device infos */
};
typedef struct _virDomainPCIAddressSet virDomainPCIAddressSet;
typedef virDomainPCIAddressSet *virDomainPCIAddressSetPtr;

/* a combination of all bit that describe the type of connections
 * allowed, e.g. PCI, PCIe, switch
 */
# define VIR_PCI_CONNECT_TYPES_MASK \
   (VIR_PCI_CONNECT_TYPE_PCI | VIR_PCI_CONNECT_TYPE_PCIE)

#endif /* __DOMAIN_ADDR_H__ */
