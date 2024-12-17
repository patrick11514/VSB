import type { DataConnector } from './data_access/connector/DataConnector';
import { JsonDBConnector } from './data_access/connector/JsonDBConnector';
import { MariaDBConnector } from './data_access/connector/MariaDBConnector';

export class GlobalConnector {
    static connector: DataConnector = new MariaDBConnector();
}
