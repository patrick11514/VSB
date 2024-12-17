import type { DataConnector } from './data_access/connector/DataConnector';
import { MariaDBConnector } from './data_access/connector/MariaDBConnector';

export class GlobalConnector {
    static jsonDBPath = 'database';
    static connector: DataConnector = new MariaDBConnector();
}
